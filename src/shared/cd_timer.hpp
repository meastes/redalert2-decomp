// Westwood's countdown timer and its time sources (moved out of
// shared/ms_anim.hpp, which keeps SystemTimerClass): Started is -1 while
// stopped; FrameTimerClass counts game frames (0x00A40D2C).
#ifndef RA2_SHARED_CD_TIMER_HPP
#define RA2_SHARED_CD_TIMER_HPP

class NoInitClass;

extern int Frame;                       // 0x00A40D2C game frame counter

class FrameTimerClass {
public:
    operator long() const { return Frame; }
};

#ifdef RA2_CDTIMER_BASIC_BASE
// Westwood's timer hierarchy (TibSun/YR timer.h): BasicTimerClass holds the
// start stamp and the time source and has a user destructor, CDTimerClass
// derives from it. The layout is unchanged; the base makes the NoInitClass
// constructor a charged /Ob2 site, which is what keeps the VectorClass
// constructors out of line in TechnoClass(NoInitClass) 0x006C1F10 (ra2-decomp's
// headers reproduce it the same way).
template <class T>
class BasicTimerClass {
public:
    BasicTimerClass(long set = 0) : Started(Timer - set) {}
    BasicTimerClass(NoInitClass const &);
    ~BasicTimerClass();

    long Started;           // +0x00
    T Timer;                // +0x04
};

template <class T>
inline BasicTimerClass<T>::BasicTimerClass(NoInitClass const &)
{
}

template <class T>
inline BasicTimerClass<T>::~BasicTimerClass()
{
}

template <class T>
class CDTimerClass : public BasicTimerClass<T> {
public:
    CDTimerClass() : BasicTimerClass<T>(0), DelayTime(0) {}
    CDTimerClass(long set) : BasicTimerClass<T>(0), DelayTime(set) {}
    CDTimerClass(NoInitClass const &noinit) : BasicTimerClass<T>(noinit) {}
#else
template <class T>
class CDTimerClass {
public:
    CDTimerClass() : Started(Timer), DelayTime(0) {}
#ifdef RA2_CDTIMER_NAMED_DURATION
    CDTimerClass(long set) {
        long started = Timer;
        long duration = set;
        Started = started;
        DelayTime = duration;
    }
#else
    CDTimerClass(long set) : Started(Timer), DelayTime(set) {}
#endif
    CDTimerClass(NoInitClass const &noinit) {}
#endif
    // Empty but user-declared (TS's TTimerClass has ~TTimerClass() {}): the
    // static timers of Scroll.cpp (0x00AB7048) and Mouse.cpp (0x00A70F70)
    // register atexit destructors that only set their guard bits.
    ~CDTimerClass() {}

    // The time left (Westwood's operator long); UnknownStage::Graphic_Logic
    // tests `Timer == 0` through it (TerrainClass::AI 0x006E3F10).
#ifdef RA2_CDTIMER_CONVERSION_BODY
    operator long() const {
        long remain=DelayTime;
        if(Started!=0xFFFFFFFFU) {
            long elapsed=Timer-Started;
            if(elapsed<remain) return remain-elapsed;
            return 0;
        }
        return remain;
    }
#elif defined(RA2_CDTIMER_CONVERSION_VALUE_BODY)
    // TibSun timer.h's operator long: Value's body written out again, so
    // Techno.CPP's UnknownStage::Graphic_Logic calls its own COMDAT (0x00453EB0).
    operator long() const
    {
        long remain = DelayTime;
        if (Started != 0xFFFFFFFFU) {
            long value = Timer - Started;
            if (value < remain) {
                return remain - value;
            } else {
                return 0;
            }
        }
        return remain;
    }
    // TibSun timer.h's function operator, the same count again (TechnoClass
    // 0x006D8820 calls it out of line as 0x004A4D20).
    long operator()() const
    {
        long remain = DelayTime;
        if (Started != 0xFFFFFFFFU) {
            long value = Timer - Started;
            if (value < remain) {
                return remain - value;
            } else {
                return 0;
            }
        }
        return remain;
    }
#else
    operator long() const { return Value(); }
#endif
    // Value tests Started against 0xFFFFFFFFU as Westwood's FTIMER.H does (an
    // unsigned test): VC6 then keeps it after Stop's signed test, as
    // TActionClass 0x006B0A60 does (stop the mission timer).
#ifdef RA2_CDTIMER_VALUE_SOURCE
    long Value() const;
#else
    long Value() const
    {
        long remain = DelayTime;
        if (Started != 0xFFFFFFFFU) {
            long value = Timer - Started;
            if (value < remain) {
                return remain - value;
            } else {
                return 0;
            }
        }
        return remain;
    }
#endif
    bool Is_Active() const { return Started != -1; }
#ifdef RA2_CDTIMER_STOP_SOURCE
    // The owning unit supplies the original out-of-line specialization.
    void Stop();
#else
    void Stop()
    {
        if (Started != -1) {
            DelayTime = Value();
            Started = -1;
        }
    }
#endif
    void Start()
    {
        if (Started == -1) {
            Started = Timer;
        }
    }

#ifndef RA2_CDTIMER_BASIC_BASE
    long Started;           // +0x00
    T Timer;                // +0x04
#endif
    long DelayTime;         // +0x08
};

// A frame timer that TechnoClass 0x006D7990 restarts at 25 and Scenario.cpp's
// post-load refresh 0x0065D2C0 restarts from the current frame (0x00AC0178;
// declared here so units without the TechnoClass layout can use it).
extern CDTimerClass<FrameTimerClass> unknown_timer_ac0178;  // 0x00AC0178

#endif
