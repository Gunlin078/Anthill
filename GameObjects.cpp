#include "GameObjects.h"

QList<Resource*> ResourceManager::pendingResources_;

void ResourceManager::scheduleRestoration(Resource* res) {
    if (!res || pendingResources_.contains(res)) return;
    res->restorationDeadline_ = QDateTime::currentSecsSinceEpoch() + Config::CREATION_INTERVAL;
    pendingResources_.append(res);
}

void ResourceManager::checkRestorations(long long currentTimeSec) {
    for (auto it = pendingResources_.begin(); it != pendingResources_.end(); ) {
        Resource* res = *it;
        if (currentTimeSec >= res->restorationDeadline_) {
            res->restore();
            it = pendingResources_.erase(it);
        } else {
            ++it;
        }
    }
}

void ResourceManager::clearAll() {
    pendingResources_.clear();
}

void Resource::remove() {
    if (isActive_) {
        isActive_ = false;
        count_ = 0;
        ResourceManager::scheduleRestoration(this);
    }
}

void Resource::restore() {
    isActive_ = true;
    count_ = initialCount_;
    restorationDeadline_ = 0;
}

int Resource::pickUpPart(int amount)
{
    if (amount > count_){
        int received = count_;
        remove();
        return received;
    }
    else{
        count_ -= amount;
        return amount;
    }
}
