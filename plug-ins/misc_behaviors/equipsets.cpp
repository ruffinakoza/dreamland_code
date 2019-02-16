#include "equipsets.h"
#include <map>

#include "affect.h"
#include "room.h"
#include "pcharacter.h"
#include "character.h"
#include "object.h"

#include "dreamland.h"
#include "merc.h"
#include "mercdb.h"
#include "act.h"
#include "magic.h"
#include "gsn_plugin.h"
#include "effects.h"
#include "handler.h"
#include "fight.h"
#include "damage_impl.h"
#include "def.h"
   
GSN(sidhe_armor);
GSN(travellers_joy);

EquipSet::EquipSet(int size) : totalSetSize(size)
{
}
 
void EquipSet::equip( Character *victim )
{
    if (victim->is_npc())
        return;

    if (hasAffect(victim)) 
        return;

    if (!isComplete(victim)) 
        return;

    addAffect(victim);    
}

void EquipSet::remove( Character *victim )
{
    if (victim->is_npc())
        return;

    if (!hasAffect(victim))
        return;

    if (isComplete(victim))
        return;

    removeAffect(victim);
}

void EquipSet::fight( Character *ch )
{
}

bool EquipSet::isComplete(Character *ch) const
{
    GlobalArray slots(wearlocationManager);
    int wornSetSize = 0;

    if (ch->is_npc())
        return false;
    
    if (totalSetSize <= 0)
        return false;

    for (Object *item = ch->carrying; item; item = item->next_content) {
        if (item->wear_loc == wear_none)
            continue;
        if (!item->behavior)
            continue;
        if (item->behavior->getType() != obj->behavior->getType())
            continue;

        slots[item->wear_loc] = item->pIndexData->vnum;
        wornSetSize++;
    }

    int neck1 = slots[wear_neck_1];
    int neck2 = slots[wear_neck_2];
    if (neck1 != 0 && neck1 == neck2)
        wornSetSize--;

    int wrist1 = slots[wear_wrist_l];
    int wrist2 = slots[wear_wrist_r];
    if (wrist1 != 0 && wrist1 == wrist2)
        wornSetSize--;
    
    if (ch->is_immortal())
        ch->pecho("Окончательный результат %d предмета из %d.", wornSetSize, totalSetSize);

    return wornSetSize >= totalSetSize;
}

SidheArmorSet::SidheArmorSet()
                : EquipSet(5), sn(gsn_sidhe_armor)
{
}

bool SidheArmorSet::hasAffect(Character *ch) const
{
    return ch->isAffected(sn);
}

void SidheArmorSet::addAffect(Character *ch) const
{
    if (IS_NEUTRAL(ch)) {
        ch->pecho("Набор сидхийской брони на мгновение загорается тусклым светом и тут же гаснет.");
        return;
    }

    Affect af;

    af.type      = sn;
    af.level     = ch->getModifyLevel();
    af.duration  = -2;

    af.where     = TO_AFFECTS;
    af.bitvector = IS_EVIL(ch) ? AFF_PROTECT_GOOD : AFF_PROTECT_EVIL;

    af.modifier  =  20;
    af.location  = APPLY_HITROLL;
    affect_to_char(ch, &af);

    af.where     = TO_RESIST;
    af.bitvector = ch->getRace()->getVuln().isSet(VULN_IRON) ? RES_IRON : 0;
    af.modifier  =  30;
    af.location  = APPLY_DAMROLL;
    affect_to_char(ch, &af);

    af.where     = TO_VULN;
    af.bitvector = IS_EVIL(ch) ? VULN_HOLY : VULN_NEGATIVE;
    af.modifier  =  200;
    af.location  = APPLY_HIT;
    affect_to_char(ch, &af);

    ch->pecho("{WНа{wб{Wо{wр{W си{wдх{Wийск{wо{Wй б{wр{Wони на{wч{Wина{wет{W светиться ровным се{wр{Wе{wбр{Wист{wы{Wм светом.{x");
}

void SidheArmorSet::removeAffect(Character *ch) const
{
    ch->pecho("Свет, окружавший твою сидхийскую броню, мерцает и гаснет.");
    affect_strip(ch, sn);
}

TravellersJoySet::TravellersJoySet()
                : EquipSet(4), sn(gsn_travellers_joy)
{
}

bool TravellersJoySet::hasAffect(Character *ch) const
{
    return ch->isAffected(sn);
}

void TravellersJoySet::addAffect(Character *ch) const
{
    Affect af;

    af.type      = sn;
    af.level     = ch->getModifyLevel();
    af.duration  = -2;
    af.where     = TO_AFFECTS;
    af.bitvector = 0;

    af.modifier  =  8;
    af.location  = APPLY_HITROLL;
    affect_to_char(ch, &af);

    af.modifier  =  12;
    af.location  = APPLY_DAMROLL;
    affect_to_char(ch, &af);

    af.modifier  =  35;
    af.location  = APPLY_HIT;
    affect_to_char(ch, &af);

    af.modifier  =  2;
    af.location  = APPLY_DEX;
    affect_to_char(ch, &af);

    af.modifier  =  2;
    af.location  = APPLY_STR;
    affect_to_char(ch, &af);

    af.modifier  =  2;
    af.location  = APPLY_CON;
    affect_to_char(ch, &af);

    ch->pecho("{CКомплект одежд путешественника начитает светиться ровным голубоватым светом.{x");
}

void TravellersJoySet::removeAffect(Character *ch) const
{
    ch->pecho("Свет, окружавший комплект одежд путешественника, мерцает и гаснет.");
    affect_strip(ch, sn);
}
