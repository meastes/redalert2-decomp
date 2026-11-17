// SPDX-License-Identifier: GPL-3.0-or-later

#include "audcache.h"
#include <string.h>

AudioCache * __fastcall AudioCacheCreate(AudioIDXData * bag, int cacheSize, int maxItems, int frameSize)
{
    AudioCache * cache = new AudioCache;
    memset(cache, 0, sizeof(*cache));
    cache->bag = bag;
    cache->frameSize = frameSize;
    int bytes = frameSize + sizeof(AudioFrame);
    cache->framePool = MemoryPoolCreate(cacheSize / bytes, bytes);
    cache->itemPool = MemoryPoolCreate(maxItems, sizeof(AudioCacheItem));
    ListInit(&cache->items);
    if (!cache->framePool || !cache->itemPool) {
        AudioCacheDestroy(cache);
        cache = 0;
    }
    return cache;
}

void __fastcall AudioCacheDestroy(AudioCache * cache)
{
    AudioCacheItem * item;
    while ((item = (AudioCacheItem *)ListNodeNext(&cache->items)) != 0) {
        AudioCacheItemFree(item);
    }
    if (cache->framePool) MemoryPoolDestroy(cache->framePool);
    if (cache->itemPool) MemoryPoolDestroy(cache->itemPool);
    delete cache;
}

AudioCacheItem * __fastcall AudioCacheGetItem(AudioCache * cache, int id)
{
    for (ListNode * node = cache->items.next; node != &cache->items; node = node->next) {
        AudioCacheItem * item = (AudioCacheItem *)node;
        if (item->id == id && item->valid) return item;
    }
    return 0;
}

void __fastcall AudioCacheInvalidate(AudioCache * cache)
{
    for (ListNode * node = cache->items.next; node != &cache->items; node = node->next) {
        ((AudioCacheItem *)node)->valid = 0;
    }
}

AudioCacheItem * __fastcall AudioCacheLoadItem(AudioCache * cache, int id)
{
    AudioCacheItem * item = AudioCacheGetItem(cache, id);
    if (item) {
        ListNodeRemove(&item->nd);
        ListNodeAppend(&cache->items, &item->nd);
        return item;
    }
    if (cache->bag->OpenSample(id)) {
        item = (AudioCacheItem *)MemoryPoolGetItem(cache->itemPool);
        if (!item) {
            AudioCacheFreeOldestItem(cache);
            item = (AudioCacheItem *)MemoryPoolGetItem(cache->itemPool);
            if (!item) {
                cache->bag->ClearCurrentSample();
                return 0;
            }
        }
        item->id = id;
        item->cache = cache;
        ListNodeInit(&item->nd);
        LockInit(&item->lock);
        AudioSampleInit(&item->sample);
        AudioFormatInit(&item->format);
        item->sample.Format = &item->format;
        int remaining = cache->bag->CurrentSampleSize;
        for (;;) {
            if (!remaining) {
                cache->bag->GetSampleInformation(item->id, &item->format);
                ListNodeAppend(&cache->items, &item->nd);
                item->valid = 1;
                cache->bag->ClearCurrentSample();
                return item;
            }
            int bytes = cache->frameSize;
            if (bytes > remaining) bytes = remaining;
            AudioFrame * frame = (AudioFrame *)MemoryPoolGetItem(cache->framePool);
            while (!frame) {
                if (!AudioCacheFreeOldestItem(cache)) goto failed;
                frame = (AudioFrame *)MemoryPoolGetItem(cache->framePool);
            }
            AudioFrameInit(frame, frame + 1, bytes);
            AudioSampleAddFrame(&item->sample, frame);
            int read = cache->bag->ReadSample(frame + 1, bytes);
            if (read != bytes) break;
            remaining -= read;
        }
failed:
        AudioCacheItemFree(item);
    }
    return 0;
}

int __fastcall AudioCacheFreeOldestItem(AudioCache * cache)
{
    AudioCacheItem * item = (AudioCacheItem *)ListNodePrev(&cache->items);
    while (item) {
        if (!AudioCacheItemInUse(item)) {
            AudioCacheItemFree(item);
            return 1;
        }
        item = (AudioCacheItem *)ListNodePrev(&item->nd);
    }
    return 0;
}

void __fastcall AudioCacheItemLock(AudioCacheItem * item)
{
    LockAcquire(&item->lock);
}

void __fastcall AudioCacheItemUnlock(AudioCacheItem * item)
{
    LockRelease(&item->lock);
}

int __fastcall AudioCacheItemInUse(AudioCacheItem * item)
{
    return item->lock.count;
}

void __fastcall AudioCacheItemFree(AudioCacheItem * item)
{
    if (ListNodeInList(&item->nd)) ListNodeRemove(&item->nd);
    AudioFrame * frame;
    while ((frame = (AudioFrame *)ListNodeNext(&item->sample.Frames)) != 0) {
        ListNodeRemove(&frame->nd);
        AudioFrameDeinit(frame);
        MemoryPoolReturnItem(item->cache->framePool, frame);
    }
    AudioSampleDeinit(&item->sample);
    MemoryPoolReturnItem(item->cache->itemPool, item);
}

int __fastcall AudioCacheItemId(AudioCacheItem * item)
{
    return item->id;
}

AudioSample * __fastcall AudioCacheItemSample(AudioCacheItem * item)
{
    return &item->sample;
}
