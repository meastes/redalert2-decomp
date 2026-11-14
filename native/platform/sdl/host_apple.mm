// SPDX-License-Identifier: GPL-3.0-or-later
// macOS and iOS implementations of the host's operating-system services
// (host_os.hpp). The only host file with Apple-only calls (Scout1247; the port's
// counterpart is port/src/os_apple.cpp). Objective-C++ with ARC; no SDL and no
// Win32 port headers here (both define BOOL).
#include "host_os.hpp"

#import <Foundation/Foundation.h>
#include <TargetConditionals.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ucontext.h>
#include <unistd.h>
#include <limits.h>

#if TARGET_OS_OSX
#import <AppKit/AppKit.h>
#include <mach-o/dyld.h>
#elif TARGET_OS_IOS
#import <UIKit/UIKit.h>
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>
#import <AVFAudio/AVFAudio.h>
#include <mach/mach.h>
#include <stdio.h>
#endif

namespace hostos {

bool fault_pc(void *context, uintptr_t *pc)
{
    if (!context) return false;
#if defined(__arm64__)
    *pc = (uintptr_t)((ucontext_t *)context)->uc_mcontext->__ss.__pc;
    return true;
#elif defined(__x86_64__)
    *pc = (uintptr_t)((ucontext_t *)context)->uc_mcontext->__ss.__rip;
    return true;
#else
    return false;
#endif
}

void exclude_from_backup(const char *path)
{
#if TARGET_OS_IOS
    @autoreleasepool {
        NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:path]];
        NSError *error = nil;
        if (![url setResourceValue:@YES forKey:NSURLIsExcludedFromBackupKey error:&error])
            NSLog(@"host: cannot exclude %s from backups: %@", path, error);
    }
#else
    (void)path;
#endif
}

#if TARGET_OS_OSX
// ---- macOS -----------------------------------------------------------------

std::string executable_path()
{
    char buf[4096];
    uint32_t n = sizeof buf;
    if (_NSGetExecutablePath(buf, &n) != 0) return "";
    char real[PATH_MAX];
    return realpath(buf, real) ? real : buf;
}
// Launched as RA2.app's executable (Contents/MacOS/).
bool launched_as_app() { return executable_path().find(".app/Contents/MacOS/") != std::string::npos; }

// $HOME, not NSHomeDirectory: test runs give each launch its own HOME
// (packaging/README.md "For testing").
static std::string home()
{
    const char *h = getenv("HOME");
    return h && *h ? h : "/tmp";
}
std::string settings_dir() { return home() + "/Library/Application Support/RA2Native"; }
std::string game_write_dir() { return settings_dir() + "/Game"; }
std::string log_dir() { return home() + "/Library/Logs/RA2Native"; }

// A fresh process (a signal handler cannot run Cocoa): fork and exec this
// executable as the crash dialog.
bool start_crash_report(const char *self, const char *log)
{
    pid_t child = fork();
    if (child == 0) {
        execl(self, self, "--crash-report", log && *log ? log : "-", (char *)nullptr);
        _exit(1);
    }
    return child > 0;
}
bool crash_report_at_next_start() { return false; }

void reveal_file(const char *path)
{
    @autoreleasepool {
        NSURL *url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:path]];
        [[NSWorkspace sharedWorkspace] activateFileViewerSelectingURLs:@[url]];
    }
}

// RA2_NATIVE_NSEVENT_KEYS (test scaffolding, Scout1223): AppKit events posted into
// the application's own queue take the real keyboard's and mouse's path
// (NSApplication sendEvent, SDL's key handling and text input) without the
// Accessibility permission that posting system-wide events needs.
bool post_system_click(void *native_window, double x, double y)
{
    NSWindow *window = (__bridge NSWindow *)native_window;
    if (!window) return false;
    // Window coordinates: the origin at the bottom left of the content.
    NSPoint at = NSMakePoint(x, window.contentView.frame.size.height - y);
    static const NSEventType types[3] = {NSEventTypeMouseMoved, NSEventTypeLeftMouseDown, NSEventTypeLeftMouseUp};
    for (int step = 0; step < 3; ++step) {
        NSEvent *ev = [NSEvent mouseEventWithType:types[step] location:at modifierFlags:0 timestamp:0
                                     windowNumber:window.windowNumber context:nil eventNumber:0
                                       clickCount:step ? 1 : 0 pressure:step == 1 ? 1.0f : 0.0f];
        if (ev) [NSApp postEvent:ev atStart:NO];
    }
    return true;
}

bool post_system_keys(void *native_window, const char *text)
{
    NSWindow *window = (__bridge NSWindow *)native_window;
    if (!window) return false;
    static const char letters[] = "asdfhgzxcv?bqweryt";   // macOS virtual key codes 0-17
    for (const char *p = text; *p; ++p) {
        char c = *p;
        unsigned short code = 0;
        char chars[2] = {c, 0};
        if (c == '<') { code = 51; chars[0] = 0x7F; }
        else if (c == '|') { code = 36; chars[0] = '\r'; }   // Return (Scout1231: the in-game chat line)
        else if (const char *at = strchr(letters, c)) code = (unsigned short)(at - letters);
        else continue;
        NSString *str = [NSString stringWithUTF8String:chars];
        for (int up = 0; up < 2; ++up) {
            NSEvent *ev = [NSEvent keyEventWithType:up ? NSEventTypeKeyUp : NSEventTypeKeyDown location:NSMakePoint(10, 10)
                                      modifierFlags:0 timestamp:0 windowNumber:window.windowNumber context:nil
                                         characters:str charactersIgnoringModifiers:str isARepeat:NO keyCode:code];
            if (ev) [NSApp postEvent:ev atStart:NO];
        }
    }
    return true;
}

bool pick_import_folder(void *, void (*)(void *, const char *), void *) { return false; }
bool imports_game_files() { return false; }
bool usable_rect(void *, Rect *) { return false; }
void watch_audio_interruptions(void (*)(bool)) {}
std::string device_report() { return ""; }
bool toolbar_create(void *, ToolbarItem const *, int, void (*)(int)) { return false; }
void toolbar_set(int, bool, const char *) {}
void toolbar_place(Rect) {}
bool toolbar_hit(double, double) { return false; }
void toolbar_show(bool) {}
void skip_button_create(void *, void (*)()) {}
void skip_button_show(bool) {}
void keys_button_create(void *, void (*)()) {}
void keys_button_show(bool) {}
void keys_button_set(bool) {}
void keys_button_place(Rect) {}
bool app_active() { return false; }

#elif TARGET_OS_IOS
// ---- iOS -------------------------------------------------------------------

std::string executable_path()
{
    @autoreleasepool {
        NSString *path = NSBundle.mainBundle.executablePath;
        return path ? path.UTF8String : "";
    }
}
bool launched_as_app() { return true; }

static std::string user_dir(NSSearchPathDirectory which, const char *below)
{
    @autoreleasepool {
        NSString *base = NSSearchPathForDirectoriesInDomains(which, NSUserDomainMask, YES).firstObject;
        std::string path = base ? base.UTF8String : (std::string(getenv("HOME") ? getenv("HOME") : "/tmp") + "/Library");
        return below && *below ? path + "/" + below : path;
    }
}
std::string settings_dir() { return user_dir(NSApplicationSupportDirectory, "RA2Native"); }
std::string game_write_dir() { return user_dir(NSDocumentDirectory, "Game"); }
std::string log_dir() { return user_dir(NSLibraryDirectory, "Logs/RA2Native"); }

// No other processes on iOS: the next start shows the report.
bool start_crash_report(const char *, const char *) { return false; }
bool crash_report_at_next_start() { return true; }
void reveal_file(const char *) {}

bool post_system_click(void *, double, double) { return false; }
bool post_system_keys(void *, const char *) { return false; }
bool imports_game_files() { return true; }

}  // namespace hostos

@interface RA2ImportPicker : NSObject <UIDocumentPickerDelegate>
@property(nonatomic, strong) UIDocumentPickerViewController *controller;
@property(nonatomic) void (*done)(void *, const char *);
@property(nonatomic) void *user;
@end

namespace {
RA2ImportPicker *active_picker = nil;
}

@implementation RA2ImportPicker
- (void)finish:(const char *)path
{
    RA2ImportPicker *keep = self;
    active_picker = nil;
    if (keep.done) keep.done(keep.user, path ? path : "");
}
- (void)documentPickerWasCancelled:(UIDocumentPickerViewController *)controller
{
    [self finish:nullptr];
}
- (void)documentPicker:(UIDocumentPickerViewController *)controller didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls
{
    NSURL *url = urls.firstObject;
    if (!url) { [self finish:nullptr]; return; }
    // The chosen folder is readable inside its security scope and the file
    // provider's read coordination: the callback copies it before both end.
    BOOL scoped = [url startAccessingSecurityScopedResource];
    NSFileCoordinator *coordinator = [[NSFileCoordinator alloc] initWithFilePresenter:nil];
    NSError *error = nil;
    __block bool called = false;
    [coordinator coordinateReadingItemAtURL:url options:0 error:&error byAccessor:^(NSURL *readable) {
        called = true;
        [self finish:readable.path.UTF8String];
    }];
    if (!called) {
        NSLog(@"host: import folder not readable: %@", error);
        [self finish:nullptr];
    }
    if (scoped) [url stopAccessingSecurityScopedResource];
}
@end

namespace hostos {

// The largest rectangle of `area` that avoids `obstacle`: the area itself when
// they do not meet, else the biggest of the four strips beside the obstacle
// (landscape strips first when sizes tie).
static CGRect clear_of(CGRect area, CGRect obstacle)
{
    CGRect cut = CGRectIntersection(area, obstacle);
    if (CGRectIsNull(cut) || CGRectIsEmpty(cut)) return area;
    CGRect strips[4] = {
        CGRectMake(area.origin.x, area.origin.y, CGRectGetMinX(cut) - area.origin.x, area.size.height),   // left
        CGRectMake(CGRectGetMaxX(cut), area.origin.y, CGRectGetMaxX(area) - CGRectGetMaxX(cut), area.size.height),   // right
        CGRectMake(area.origin.x, area.origin.y, area.size.width, CGRectGetMinY(cut) - area.origin.y),    // above
        CGRectMake(area.origin.x, CGRectGetMaxY(cut), area.size.width, CGRectGetMaxY(area) - CGRectGetMaxY(cut)),   // below
    };
    CGRect best = CGRectZero;
    for (CGRect r : strips) {
        if (r.size.width <= 0 || r.size.height <= 0) continue;
        double a = r.size.width * r.size.height, b = best.size.width * best.size.height;
        if (a > b || (a == b && r.size.width >= r.size.height)) best = r;
    }
    return best;
}

bool usable_rect(void *native_window, Rect *out)
{
    UIWindow *window = (__bridge UIWindow *)native_window;
    UIView *view = window.rootViewController.view;
    if (!view) return false;
    CGRect area = UIEdgeInsetsInsetRect(view.bounds, view.safeAreaInsets);
    if (@available(iOS 27.1, *)) {
        for (UIViewReservedRegionKind *kind in @[[UIViewReservedRegionKind occlusionRegionKind], [UIViewReservedRegionKind divisionRegionKind]])
            for (UIViewReservedRegion *region in [view reservedRegionsOfKind:kind])
                if (region.active) area = clear_of(area, region.frame);
    }
    CGRect in_window = [view convertRect:area toView:nil];
    *out = Rect{in_window.origin.x, in_window.origin.y, in_window.size.width, in_window.size.height};
    return true;
}

std::string device_report()
{
    task_vm_info_data_t info = {};
    mach_msg_type_number_t count = TASK_VM_INFO_COUNT;
    unsigned long long footprint = task_info(mach_task_self(), TASK_VM_INFO, (task_info_t)&info, &count) == KERN_SUCCESS ? info.phys_footprint : 0;
    const char *thermal = "unknown";
    switch (NSProcessInfo.processInfo.thermalState) {
    case NSProcessInfoThermalStateNominal: thermal = "nominal"; break;
    case NSProcessInfoThermalStateFair: thermal = "fair"; break;
    case NSProcessInfoThermalStateSerious: thermal = "serious"; break;
    case NSProcessInfoThermalStateCritical: thermal = "critical"; break;
    }
    char text[96];
    snprintf(text, sizeof text, "memory %llu MB, thermal %s", footprint >> 20, thermal);
    return text;
}

void watch_audio_interruptions(void (*callback)(bool))
{
    static id observer = nil;
    if (observer || !callback) return;
    observer = [NSNotificationCenter.defaultCenter addObserverForName:AVAudioSessionInterruptionNotification object:nil
                                                                 queue:NSOperationQueue.mainQueue usingBlock:^(NSNotification *note) {
        NSUInteger type = [note.userInfo[AVAudioSessionInterruptionTypeKey] unsignedIntegerValue];
        callback(type == AVAudioSessionInterruptionTypeBegan);
    }];
}

}  // namespace hostos

// ---- the touch controls' toolbar (Scout1249) --------------------------------
// A view whose own empty area passes touches to the game below.
@interface RA2PassView : UIView
@end
@implementation RA2PassView
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    UIView *hit = [super hitTest:point withEvent:event];
    return hit == self ? nil : hit;
}
@end

@interface RA2Toolbar : NSObject
@property(nonatomic, strong) UIView *holder;               // follows the usable area's top left
@property(nonatomic, strong) UIVisualEffectView *more;     // the "…" button's glass circle
@property(nonatomic, strong) UIVisualEffectView *pill;     // the items' glass capsule
@property(nonatomic, strong) NSMutableArray<UIButton *> *buttons;
@property(nonatomic) BOOL open;
@property(nonatomic) void (*tapped)(int);
@property(nonatomic, strong) UIVisualEffectView *skip;      // the Skip button over a cutscene
@property(nonatomic) void (*skipTapped)();
@property(nonatomic, strong) UIVisualEffectView *keys;      // the Keys button in the menus (lane C10)
@property(nonatomic) void (*keysTapped)();
@property(nonatomic) CGRect usable;                          // the usable area, window points
@end

namespace {
RA2Toolbar *toolbar = nil;
const CGFloat kBar = 48, kGap = 8, kItem = 46;   // eight items fit beside the Skip or Keys button on the Duo
UIColor *text_colour() { return [UIColor colorWithWhite:0.95 alpha:1]; }
UIColor *accent_colour() { return [UIColor colorWithRed:0xF5 / 255.0 green:0x9E / 255.0 blue:0x8B / 255.0 alpha:1]; }
// Dark glass, as duo-vnc's toolbar (a blur before iOS 26).
UIVisualEffectView *glass_view(BOOL capsule)
{
    UIVisualEffectView *v = [[UIVisualEffectView alloc] initWithEffect:nil];
    if (@available(iOS 26.0, *)) {
        UIGlassEffect *effect = [UIGlassEffect effectWithStyle:UIGlassEffectStyleRegular];
        // Not interactive (Scout1274; user report: on the Duo the "…" button sometimes stayed
        // pressed, grey, and the toolbar did not open): interactive glass tracks touches with
        // its own recognizer, which can take the touch from the button inside it (no touch up
        // inside) and leave its pressed look on. The buttons highlight themselves.
        effect.interactive = NO;
        v.effect = effect;
        v.cornerConfiguration = [UICornerConfiguration capsuleConfiguration];
    } else {
        v.effect = [UIBlurEffect effectWithStyle:UIBlurEffectStyleSystemUltraThinMaterialDark];
        v.layer.cornerRadius = kBar / 2;
        v.clipsToBounds = YES;
    }
    (void)capsule;
    v.overrideUserInterfaceStyle = UIUserInterfaceStyleDark;
    return v;
}
}  // namespace

@implementation RA2Toolbar
- (void)toggleOpen
{
    self.open = !self.open;
    fprintf(stderr, "host: toolbar %s\n", self.open ? "opened" : "closed");
    if (self.open) self.pill.hidden = NO;
    [UIView animateWithDuration:0.25 delay:0 usingSpringWithDamping:0.85 initialSpringVelocity:0 options:0 animations:^{
        self.pill.alpha = self.open ? 1 : 0;
        self.pill.transform = self.open ? CGAffineTransformIdentity : CGAffineTransformMakeTranslation(-self.pill.bounds.size.width / 2, 0);
    } completion:^(BOOL) { if (!self.open) self.pill.hidden = YES; }];
}
- (void)itemTapped:(UIButton *)button
{
    if (self.tapped) self.tapped((int)button.tag);
}
- (void)skipPressed
{
    if (self.skipTapped) self.skipTapped();
}
- (void)keysPressed
{
    if (self.keysTapped) self.keysTapped();
}
@end

namespace hostos {

bool toolbar_create(void *native_window, ToolbarItem const *items, int count, void (*tapped)(int))
{
    UIWindow *window = (__bridge UIWindow *)native_window;
    UIView *view = window.rootViewController.view;
    if (!view || toolbar) return toolbar != nil;
    RA2Toolbar *t = [RA2Toolbar new];
    t.tapped = tapped;
    t.buttons = [NSMutableArray array];
    t.holder = [[RA2PassView alloc] initWithFrame:CGRectMake(0, 0, kBar + kGap + count * kItem + 16, kBar)];
    t.holder.backgroundColor = UIColor.clearColor;
    // The holder lets touches between its controls reach the game.
    t.holder.userInteractionEnabled = YES;
    t.more = glass_view(YES);
    t.more.frame = CGRectMake(0, 0, kBar, kBar);
    UIButton *more = [UIButton buttonWithType:UIButtonTypeSystem];
    more.frame = t.more.contentView.bounds;
    more.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    [more setImage:[UIImage systemImageNamed:@"ellipsis"] forState:UIControlStateNormal];
    more.tintColor = text_colour();
    more.accessibilityLabel = @"Controls";
    [more addTarget:t action:@selector(toggleOpen) forControlEvents:UIControlEventTouchUpInside];
    [t.more.contentView addSubview:more];
    [t.holder addSubview:t.more];
    t.pill = glass_view(YES);
    t.pill.frame = CGRectMake(kBar + kGap, 0, count * kItem + 16, kBar);
    for (int i = 0; i < count; ++i) {
        UIButton *b = [UIButton buttonWithType:UIButtonTypeSystem];
        UIButtonConfiguration *c = [UIButtonConfiguration plainButtonConfiguration];
        c.image = [UIImage systemImageNamed:[NSString stringWithUTF8String:items[i].symbol]];
        c.preferredSymbolConfigurationForImage = [UIImageSymbolConfiguration configurationWithPointSize:15 weight:UIImageSymbolWeightSemibold];
        c.title = [NSString stringWithUTF8String:items[i].title];
        c.imagePlacement = NSDirectionalRectEdgeTop;
        c.imagePadding = 2;
        c.contentInsets = NSDirectionalEdgeInsetsMake(2, 2, 2, 2);
        c.titleTextAttributesTransformer = ^NSDictionary *(NSDictionary *in) {
            NSMutableDictionary *out = [in mutableCopy];
            out[NSFontAttributeName] = [UIFont systemFontOfSize:10 weight:UIFontWeightSemibold];
            return out;
        };
        b.configuration = c;
        b.tintColor = text_colour();
        b.tag = i;
        b.frame = CGRectMake(8 + i * kItem, 2, kItem, kBar - 4);
        b.accessibilityLabel = c.title;
        [b addTarget:t action:@selector(itemTapped:) forControlEvents:UIControlEventTouchUpInside];
        [t.pill.contentView addSubview:b];
        [t.buttons addObject:b];
    }
    [t.holder addSubview:t.pill];
    t.pill.hidden = YES;
    t.pill.alpha = 0;
    t.open = NO;
    [view addSubview:t.holder];
    toolbar = t;
    // Tests: the toolbar's own actions at given times (host_os.hpp).
    if (const char *taps = getenv("RA2_NATIVE_TOOLBAR_TAPS")) {
        for (NSString *entry in [[NSString stringWithUTF8String:taps] componentsSeparatedByString:@";"]) {
            NSArray<NSString *> *parts = [entry componentsSeparatedByString:@":"];
            if (parts.count != 2) continue;
            double ms = parts[0].doubleValue;
            int item = parts[1].intValue;
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(ms * NSEC_PER_MSEC)), dispatch_get_main_queue(), ^{
                if (!toolbar) return;
                fprintf(stderr, "host: toolbar test tap %d\n", item);
                UIButton *b = nil;
                if (item == -2) {
                    // A hidden Skip button cannot be touched: the test tap does nothing then.
                    if (!toolbar.skip || toolbar.skip.hidden) { fprintf(stderr, "host: toolbar test tap -2: skip button hidden\n"); return; }
                    for (UIView *sub in toolbar.skip.contentView.subviews)
                        if ([sub isKindOfClass:UIButton.class]) b = (UIButton *)sub;
                } else if (item == -3) {
                    if (!toolbar.keys || toolbar.keys.hidden) { fprintf(stderr, "host: toolbar test tap -3: keys button hidden\n"); return; }
                    for (UIView *sub in toolbar.keys.contentView.subviews)
                        if ([sub isKindOfClass:UIButton.class]) b = (UIButton *)sub;
                } else if (item < 0) {
                    if (toolbar.holder.hidden) { fprintf(stderr, "host: toolbar test tap -1: toolbar hidden\n"); return; }
                    for (UIView *sub in toolbar.more.contentView.subviews)
                        if ([sub isKindOfClass:UIButton.class]) b = (UIButton *)sub;
                } else if (item < (int)toolbar.buttons.count) {
                    if (toolbar.holder.hidden) { fprintf(stderr, "host: toolbar test tap %d: toolbar hidden\n", item); return; }
                    b = toolbar.buttons[item];
                }
                [b sendActionsForControlEvents:UIControlEventTouchUpInside];
            });
        }
    }
    return true;
}


void toolbar_set(int index, bool on, const char *title)
{
    if (!toolbar || index < 0 || index >= (int)toolbar.buttons.count) return;
    UIButton *b = toolbar.buttons[index];
    UIButtonConfiguration *c = b.configuration;
    if (title) c.title = [NSString stringWithUTF8String:title];
    c.baseForegroundColor = on ? accent_colour() : text_colour();
    c.background.backgroundColor = on ? [accent_colour() colorWithAlphaComponent:0.22] : UIColor.clearColor;
    c.background.cornerRadius = 10;
    b.configuration = c;
    b.accessibilityValue = on ? @"on" : @"off";
}

void toolbar_place(Rect usable)
{
    if (!toolbar) return;
    UIView *view = toolbar.holder.superview;
    CGRect in_view = view ? [view convertRect:CGRectMake(usable.x, usable.y, usable.w, usable.h) fromView:nil] : CGRectZero;
    toolbar.usable = in_view;
    CGRect f = toolbar.holder.frame;
    f.origin = CGPointMake(in_view.origin.x + 8, in_view.origin.y + 8);
    toolbar.holder.frame = f;
    for (UIView *corner in @[toolbar.skip ?: NSNull.null]) {
        if (![corner isKindOfClass:UIView.class]) continue;
        CGRect k = corner.frame;
        k.origin = CGPointMake(CGRectGetMaxX(in_view) - k.size.width - 8, in_view.origin.y + 8);
        corner.frame = k;
    }
}

bool toolbar_hit(double x, double y)
{
    if (!toolbar) return false;
    CGPoint p = CGPointMake(x, y);
    // The holder's hit test answers nil between its controls (RA2PassView) and for the
    // hidden capsule; the Skip and Keys buttons sit in the top right corner.
    UIView *holder = toolbar.holder;
    if (!holder.hidden && [holder hitTest:[holder convertPoint:p fromView:nil] withEvent:nil]) return true;
    for (UIView *corner in @[toolbar.skip ?: NSNull.null, toolbar.keys ?: NSNull.null])
        if ([corner isKindOfClass:UIView.class] && !corner.hidden && corner.alpha > 0
            && [corner hitTest:[corner convertPoint:p fromView:nil] withEvent:nil] != nil)
            return true;
    return false;
}

// A glass capsule with one button at the usable area's top right (Skip, Keys), hidden until
// shown (corner_show).
const CGFloat kCornerWidth = 96;
UIVisualEffectView *corner_button(NSString *symbol, NSString *title, NSString *label, SEL action, NSDirectionalRectEdge image_edge)
{
    UIView *view = toolbar.holder.superview;
    UIVisualEffectView *v = glass_view(YES);
    v.frame = CGRectMake(0, 0, kCornerWidth, kBar);
    UIButton *b = [UIButton buttonWithType:UIButtonTypeSystem];
    UIButtonConfiguration *c = [UIButtonConfiguration plainButtonConfiguration];
    c.image = [UIImage systemImageNamed:symbol];
    c.preferredSymbolConfigurationForImage = [UIImageSymbolConfiguration configurationWithPointSize:14 weight:UIImageSymbolWeightSemibold];
    c.title = title;
    c.imagePlacement = image_edge;
    c.imagePadding = 6;
    c.titleTextAttributesTransformer = ^NSDictionary *(NSDictionary *in) {
        NSMutableDictionary *out = [in mutableCopy];
        out[NSFontAttributeName] = [UIFont systemFontOfSize:15 weight:UIFontWeightSemibold];
        return out;
    };
    b.configuration = c;
    b.tintColor = text_colour();
    b.frame = v.contentView.bounds;
    b.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    b.accessibilityLabel = label;
    [b addTarget:toolbar action:action forControlEvents:UIControlEventTouchUpInside];
    [v.contentView addSubview:b];
    v.hidden = YES;
    v.alpha = 0;
    [view addSubview:v];
    CGRect u = toolbar.usable;
    if (!CGRectIsEmpty(u)) v.frame = CGRectMake(CGRectGetMaxX(u) - kCornerWidth - 8, u.origin.y + 8, kCornerWidth, kBar);
    return v;
}
// Shown or hidden with a short fade (hidden once faded, so it takes no touches).
void fade_show(UIView *v, bool show)
{
    if (!v) return;
    [v.superview bringSubviewToFront:v];
    if (show) v.hidden = NO;
    [UIView animateWithDuration:0.2 animations:^{ v.alpha = show ? 1 : 0; }
                     completion:^(BOOL) { if (!show && v.alpha == 0) v.hidden = YES; }];
}

void skip_button_create(void *, void (*tapped)())
{
    if (!toolbar || toolbar.skip) return;
    toolbar.skip = corner_button(@"forward.end.fill", @"Skip", @"Skip movie", @selector(skipPressed), NSDirectionalRectEdgeTrailing);
    toolbar.skipTapped = tapped;
}

void skip_button_show(bool show)
{
    if (toolbar) fade_show(toolbar.skip, show);
}

void keys_button_create(void *, void (*tapped)())
{
    if (!toolbar || toolbar.keys) return;
    toolbar.keys = corner_button(@"keyboard", @"Keys", @"Keyboard", @selector(keysPressed), NSDirectionalRectEdgeLeading);
    toolbar.keysTapped = tapped;
}

void keys_button_show(bool show)
{
    if (toolbar) fade_show(toolbar.keys, show);
}

void keys_button_place(Rect r)
{
    if (!toolbar || !toolbar.keys) return;
    UIView *view = toolbar.keys.superview;
    CGRect f = CGRectMake(r.x, r.y, r.w, r.h);
    if (view) f = [view convertRect:f fromView:nil];
    toolbar.keys.frame = f;
    // Narrower than the capsule (a side band): the keyboard symbol alone, a glass circle.
    for (UIView *sub in toolbar.keys.contentView.subviews) {
        if (![sub isKindOfClass:UIButton.class]) continue;
        UIButton *b = (UIButton *)sub;
        UIButtonConfiguration *c = b.configuration;
        c.title = r.w < kCornerWidth ? nil : @"Keys";
        b.configuration = c;
    }
}

void keys_button_set(bool on)
{
    if (!toolbar || !toolbar.keys) return;
    for (UIView *sub in toolbar.keys.contentView.subviews) {
        if (![sub isKindOfClass:UIButton.class]) continue;
        UIButton *b = (UIButton *)sub;
        UIButtonConfiguration *c = b.configuration;
        c.baseForegroundColor = on ? accent_colour() : text_colour();
        b.configuration = c;
        b.accessibilityValue = on ? @"on" : @"off";
    }
}

void toolbar_show(bool show)
{
    if (toolbar) fade_show(toolbar.holder, show);
}

bool app_active()
{
    return UIApplication.sharedApplication.applicationState == UIApplicationStateActive;
}

bool pick_import_folder(void *native_window, void (*done)(void *, const char *), void *user)
{
    UIWindow *window = (__bridge UIWindow *)native_window;
    UIViewController *parent = window.rootViewController;
    if (!parent || active_picker) return false;
    while (parent.presentedViewController) parent = parent.presentedViewController;
    RA2ImportPicker *picker = [RA2ImportPicker new];
    picker.done = done;
    picker.user = user;
    picker.controller = [[UIDocumentPickerViewController alloc] initForOpeningContentTypes:@[UTTypeFolder] asCopy:NO];
    picker.controller.delegate = picker;
    picker.controller.allowsMultipleSelection = NO;
    // SDL's view stays attached under the picker (Outpost 2's iOS host found the
    // root view resized after a sheet otherwise).
    picker.controller.modalPresentationStyle = UIModalPresentationOverFullScreen;
    active_picker = picker;
    [parent presentViewController:picker.controller animated:YES completion:nil];
    return true;
}

#endif

}  // namespace hostos
