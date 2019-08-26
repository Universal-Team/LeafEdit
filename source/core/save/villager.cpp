#include "villager.h"

Villager::Villager() { }

Villager::~Villager() {
    // Checking if a pointer is null is not necessary for delete. It will verify the memory is valid before attempting to delete it.
    if (this->m_villagerData != nullptr) {
        delete this->m_villagerData;
    }
}

Villager::Villager(const u32 offset, const u32 index) : m_villagerData(new Villager_s), m_offset(offset), m_index(index) {
    Save::Instance()->ReadArray(reinterpret_cast<u8 *>(this->m_villagerData), offset, sizeof(Villager_s));
}

u32 Villager::GetOffset() const {
    return this->m_offset;
}

u32 Villager::GetIndex() const {
    return this->m_index;
}

u16 Villager::GetId() const {
    return this->m_villagerData->Id;
}

void Villager::SetId(const u16 id) {
    this->m_villagerData->Id = id;
}

void Villager::Write() {
    Save::Instance()->Write(m_offset, reinterpret_cast<u8 *>(this->m_villagerData), sizeof(Villager_s));
}