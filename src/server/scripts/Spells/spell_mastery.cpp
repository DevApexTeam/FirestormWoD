/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Scripts for spells with MASTERY.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_mastery_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum MasterySpells
{
    MASTERY_SPELL_LIGHTNING_BOLT        = 45284,
    MASTERY_SPELL_CHAIN_LIGHTNING       = 45297,
    MASTERY_SPELL_LAVA_BURST            = 77451,
    MASTERY_SPELL_ELEMENTAL_BLAST       = 120588,
    MASTERY_SPELL_HAND_OF_LIGHT         = 96172,
    MASTERY_SPELL_IGNITE_AURA           = 12654,
    MASTERY_SPELL_BLOOD_SHIELD          = 77535,
    MASTERY_SPELL_DISCIPLINE_SHIELD     = 77484,
    SPELL_DK_SCENT_OF_BLOOD             = 50421,
    SPELL_MAGE_MASTERY_ICICLES          = 76613,
    SPELL_MAGE_ICICLE_DAMAGE            = 148022,
    SPELL_MAGE_ICICLE_PERIODIC_TRIGGER  = 148023,
    SPELL_PRIEST_ECHO_OF_LIGHT          = 77489,
    SPELL_WARRIOR_WEAPONS_MASTER        = 76838,
    MASTERY_SPELL_IGNITE                = 12846
};

///< Mastery: Sniper Training - 76659
class spell_mastery_sniper_training : public SpellScriptLoader
{
    public:
        spell_mastery_sniper_training() : SpellScriptLoader("spell_mastery_sniper_training") { }

        enum Masteries
        {
            RecentlyMoved       = 168809,
            SniperTrainingAura  = 168811
        };

        class spell_mastery_sniper_training_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_sniper_training_AuraScript);

            void OnUpdate(uint32, AuraEffectPtr)
            {
                if (!GetUnitOwner())
                    return;

                if (Player* l_Player = GetUnitOwner()->ToPlayer())
                {
                    if (AuraPtr l_Aura = l_Player->GetAura(Masteries::SniperTrainingAura))
                    {
                        if (l_Player->isMoving() && l_Aura->GetDuration() == -1)
                        {
                            l_Aura->SetDuration(6000);
                            l_Aura->SetMaxDuration(6000);
                        }
                        else if (!l_Player->isMoving() && l_Aura->GetDuration() != -1 && !l_Player->HasAura(Masteries::RecentlyMoved))
                            l_Player->CastSpell(l_Player, Masteries::RecentlyMoved, true);
                    }
                    else if (!l_Player->isMoving() && !l_Player->HasAura(Masteries::RecentlyMoved))
                        l_Player->CastSpell(l_Player, Masteries::RecentlyMoved, true);
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_mastery_sniper_training_AuraScript::OnUpdate, EFFECT_2, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_sniper_training_AuraScript();
        }
};

///< Sniper Training: Recently Moved - 168809
class spell_mastery_recently_moved : public SpellScriptLoader
{
    public:
        spell_mastery_recently_moved() : SpellScriptLoader("spell_mastery_recently_moved") { }

        enum Masteries
        {
            SniperTrainingAura = 168811
        };

        class spell_mastery_recently_moved_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_recently_moved_AuraScript);

            void OnRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (!GetCaster())
                    return;

                Unit* l_Caster = GetCaster();
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode == AuraRemoveMode::AURA_REMOVE_BY_EXPIRE)
                {
                    if (Player* l_Player = l_Caster->ToPlayer())
                    {
                        float l_Mastery = l_Player->GetFloatValue(EPlayerFields::PLAYER_FIELD_MASTERY) * 0.5f;
                        int32 l_BasePoints = l_Mastery;

                        l_Player->CastCustomSpell(l_Player, Masteries::SniperTrainingAura, &l_BasePoints, &l_BasePoints, &l_BasePoints, &l_BasePoints, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_mastery_recently_moved_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_recently_moved_AuraScript();
        }
};

///< Sniper Training - 168811
class spell_mastery_sniper_training_aura : public SpellScriptLoader
{
    public:
        spell_mastery_sniper_training_aura() : SpellScriptLoader("spell_mastery_sniper_training_aura") { }

        class spell_mastery_sniper_training_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_sniper_training_aura_AuraScript);

            void OnUpdate(uint32, AuraEffectPtr p_AurEff)
            {
                if (!GetUnitOwner())
                    return;

                if (Player* l_Player = GetUnitOwner()->ToPlayer())
                {
                    float l_Mastery = l_Player->GetFloatValue(EPlayerFields::PLAYER_FIELD_MASTERY) * 0.5f;
                    int32 l_BasePoints = l_Mastery;

                    if (AuraPtr l_Aura = p_AurEff->GetBase())
                    {
                        for (uint8 l_I = 0; l_I < 4; ++l_I)
                        {
                            if (AuraEffectPtr l_Effect = l_Aura->GetEffect(l_I))
                                l_Effect->ChangeAmount(l_BasePoints);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_mastery_sniper_training_aura_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_sniper_training_aura_AuraScript();
        }
};

// Mastery: Echo of Light - 77485
class spell_mastery_echo_of_light: public SpellScriptLoader
{
    public:
        spell_mastery_echo_of_light() : SpellScriptLoader("spell_mastery_echo_of_light") { }

        class spell_mastery_echo_of_light_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_echo_of_light_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                if (!eventInfo.GetHealInfo() || !eventInfo.GetHealInfo()->GetHeal())
                    return;

                Unit* unitTarget = eventInfo.GetActionTarget();
                Player* plr = GetCaster()->ToPlayer();
                if (!unitTarget || !plr)
                    return;

                float Mastery = plr->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.25f / 100.0f;
                int32 bp = (Mastery * eventInfo.GetHealInfo()->GetHeal()) / 6;

                bp += unitTarget->GetRemainingPeriodicAmount(plr->GetGUID(), SPELL_PRIEST_ECHO_OF_LIGHT, SPELL_AURA_PERIODIC_HEAL);
                plr->CastCustomSpell(unitTarget, SPELL_PRIEST_ECHO_OF_LIGHT, &bp, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_mastery_echo_of_light_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_echo_of_light_AuraScript();
        }
};

const int IcicleAuras[5] = { 148012, 148013, 148014, 148015, 148016 };
const int IcicleHits[5] = { 148017, 148018, 148019, 148020, 148021 };
bool IcicleOverstack = false;

// Called by Frostbolt - 116 and Frostfire bolt - 44614
// Mastery: Icicles - 76613
class spell_mastery_icicles: public SpellScriptLoader
{
    public:
        spell_mastery_icicles() : SpellScriptLoader("spell_mastery_icicles") { }

        class spell_mastery_icicles_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_icicles_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // Calculate damage
                        int32 hitDamage = GetHitDamage();
                        if (GetSpell()->IsCritForTarget(target))
                            hitDamage *= 2;
                        float Mastery = (_player->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f) / 100.0f;
                        hitDamage *= Mastery;

                        int8 currentMaxAura = 0;

                        // if hitDamage == 0 we have a miss, so we need to except this variant
                        if (_player->HasAura(SPELL_MAGE_MASTERY_ICICLES) && hitDamage != 0)
                        {
                            // We need to find how much icicles we have, and which is the last.
                            for (int i = 0; i < 5; i++)
                                if (_player->HasAura(IcicleAuras[i]))
                                    currentMaxAura = i+1;

                            switch (currentMaxAura)
                            {
                                case 0:
                                    _player->AddAura(IcicleAuras[0], _player);
                                    break;
                                case 1:
                                    _player->AddAura(IcicleAuras[1], _player);
                                    break;
                                case 2:
                                    _player->AddAura(IcicleAuras[2], _player);
                                    break;
                                case 3:
                                    _player->AddAura(IcicleAuras[3], _player);
                                    break;
                                case 4:
                                    _player->AddAura(IcicleAuras[4], _player);
                                    break;
                                case 5:
                                {
                                    // We need to find an icicle, with the smallest duration.
                                    int32 minDuration = 0;
                                    int32 smallestIcicle = 0;
                                    if (AuraPtr icicleCurrentAura = _player->GetAura(IcicleAuras[smallestIcicle]))
                                        minDuration = _player->GetAura(IcicleAuras[0])->GetDuration();

                                    for (int i = 1; i < 5; i++)
                                    {
                                        if (AuraPtr tmpCurrentAura = _player->GetAura(IcicleAuras[i]))
                                        {
                                            if (minDuration > tmpCurrentAura->GetDuration())
                                            {
                                                minDuration = tmpCurrentAura->GetDuration();
                                                smallestIcicle = i;
                                            }
                                        }
                                    }

                                    if (AuraPtr icicleCurrentAura = _player->GetAura(IcicleAuras[smallestIcicle]))
                                    {
                                        int32 basepoints = icicleCurrentAura->GetEffect(0)->GetAmount();
                                        _player->CastSpell(target, IcicleHits[smallestIcicle], true);
                                        _player->CastCustomSpell(target, SPELL_MAGE_ICICLE_DAMAGE, &basepoints, NULL, NULL, true);
                                        _player->RemoveAura(IcicleAuras[smallestIcicle]);
                                    }

                                    _player->AddAura(IcicleAuras[smallestIcicle], _player);

                                    if (AuraPtr icicleCurrentAura = _player->GetAura(IcicleAuras[smallestIcicle]))
                                        icicleCurrentAura->GetEffect(0)->SetAmount(int32(hitDamage));

                                    IcicleOverstack = true;
                                    break;
                                }
                            }

                            if (IcicleOverstack == false)
                                if (AuraPtr icicleCurrentAura = _player->GetAura(IcicleAuras[currentMaxAura]))
                                    icicleCurrentAura->GetEffect(0)->SetAmount(hitDamage);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mastery_icicles_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_icicles_SpellScript();
        }
};

// Ice Lance - 30455
class spell_mastery_icicles_trigger: public SpellScriptLoader
{
    public:
        spell_mastery_icicles_trigger() : SpellScriptLoader("spell_mastery_icicles_trigger") { }

        class spell_mastery_icicles_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_icicles_trigger_SpellScript);

            void HandleOnHit(SpellEffIndex effIndex)
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(SPELL_MAGE_ICICLE_PERIODIC_TRIGGER))
                        return;

                    if (GetHitUnit())
                        caster->SetIciclesTarget(GetHitUnit()->GetGUID());

                    caster->CastSpell(caster, SPELL_MAGE_ICICLE_PERIODIC_TRIGGER, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_mastery_icicles_trigger_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_icicles_trigger_SpellScript();
        }
};

// Icicles (periodic) - 148023
class spell_mastery_icicles_periodic: public SpellScriptLoader
{
    public:
        spell_mastery_icicles_periodic() : SpellScriptLoader("spell_mastery_icicles_periodic") { }

        class spell_mastery_icicles_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_icicles_periodic_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (Unit* caster = GetCaster())
                {
                    if (AuraEffectPtr aura = caster->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0))
                    {
                        if (aura->GetAmount() > 4)
                            caster->RemoveAura(GetSpellInfo()->Id);

                        // Maybe not the good target selection ...
                        if (Unit* target = ObjectAccessor::FindUnit(caster->GetIciclesTarget()))
                        {
                            if (aura->GetAmount() < 5)
                            {
                                if (AuraPtr icicleCurrentAura = caster->GetAura(IcicleAuras[aura->GetAmount()]))
                                {
                                    int32 basepoints = icicleCurrentAura->GetEffect(0)->GetAmount();
                                    caster->CastSpell(target, IcicleHits[aura->GetAmount()], true);
                                    caster->CastCustomSpell(target, SPELL_MAGE_ICICLE_DAMAGE, &basepoints, NULL, NULL, true);
                                    caster->RemoveAura(IcicleAuras[aura->GetAmount()]);
                                    aura->SetAmount(aura->GetAmount() + 1);
                                }
                            }

                            IcicleOverstack = false;
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_mastery_icicles_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_icicles_periodic_AuraScript();
        }
};

// Icicles: 148017, 148018, 148019, 148020, 148021
class spell_mastery_icicles_hit: public SpellScriptLoader
{
    public:
        spell_mastery_icicles_hit() : SpellScriptLoader("spell_mastery_icicles_hit") { }

        class spell_mastery_icicles_hit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_icicles_hit_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->GetGUID() != _player->GetGUID())
                        {
                            int8 currentMinAura = 0;
                            if (IcicleOverstack == false)
                            {
                                // We need to find the first icicle and if we found it - break, because function will be called one more time.
                                for (int i = 4; i >= 0; i--)
                                {
                                    if (AuraPtr icicleCurrentAura = _player->GetAura(IcicleAuras[i]))
                                    {
                                        int32 basepoints = icicleCurrentAura->GetEffect(0)->GetAmount();
                                        _player->CastSpell(target, IcicleHits[i], true);
                                        _player->CastCustomSpell(target, SPELL_MAGE_ICICLE_DAMAGE, &basepoints, NULL, NULL, true);
                                        _player->RemoveAura(IcicleAuras[i]);
                                        break;
                                    }
                                }
                            }

                            IcicleOverstack = false;
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mastery_icicles_hit_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_icicles_hit_SpellScript();
        }
};

// Called by Power Word : Shield - 17, Power Word : Shield (Divine Insight) - 123258, Spirit Shell - 114908, Angelic Bulwark - 114214 and Divine Aegis - 47753
// Mastery : Shield Discipline - 77484
class spell_mastery_shield_discipline: public SpellScriptLoader
{
    public:
        spell_mastery_shield_discipline() : SpellScriptLoader("spell_mastery_shield_discipline") { }

        class spell_mastery_shield_discipline_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_shield_discipline_AuraScript);

            void CalculateAmount(constAuraEffectPtr , int32 & p_Amount, bool & )
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(MASTERY_SPELL_DISCIPLINE_SHIELD) && caster->getLevel() >= 80)
                    {
                        float l_Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.625f;
                        p_Amount += CalculatePct(p_Amount, l_Mastery);
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_mastery_shield_discipline_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_shield_discipline_AuraScript();
        }
};

// Called by 45470 - Death Strike (Heal)
// 77513 - Mastery : Blood Shield
class spell_mastery_blood_shield: public SpellScriptLoader
{
    public:
        spell_mastery_blood_shield() : SpellScriptLoader("spell_mastery_blood_shield") { }

        class spell_mastery_blood_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_blood_shield_SpellScript);

            void HandleAfterHit()
            {
                if (Player* _plr = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_plr->GetTypeId() == TYPEID_PLAYER && _plr->HasAura(77513) && _plr->getLevel() >= 80)
                        {
                            // Check the Mastery aura while in Blood presence
                            if (_plr->HasAura(77513) && _plr->HasAura(48263))
                            {
                                float Mastery = _plr->GetFloatValue(PLAYER_FIELD_MASTERY) * 6.25f / 100.0f;

                                int32 bp = -int32(GetHitDamage() * Mastery);

                                if (AuraPtr scentOfBlood = _plr->GetAura(SPELL_DK_SCENT_OF_BLOOD))
                                    AddPct(bp, (scentOfBlood->GetStackAmount() * 20));

                                if (AuraEffectPtr bloodShield = target->GetAuraEffect(MASTERY_SPELL_BLOOD_SHIELD, EFFECT_0))
                                    bp += bloodShield->GetAmount();

                                bp = std::min(uint32(bp), target->GetMaxHealth());

                                _plr->CastCustomSpell(target, MASTERY_SPELL_BLOOD_SHIELD, &bp, NULL, NULL, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_mastery_blood_shield_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_blood_shield_SpellScript();
        }
};

// Called by 133 - Fireball, 44614 - Frostfire Bolt, 108853 - Inferno Blast, 2948 - Scorch and 11366 - Pyroblast
// 12846 - Mastery : Ignite
class spell_mastery_ignite: public SpellScriptLoader
{
    public:
        spell_mastery_ignite() : SpellScriptLoader("spell_mastery_ignite") { }

        class spell_mastery_ignite_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_ignite_SpellScript);

            void HandleAfterHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster->GetTypeId() == TYPEID_PLAYER && l_Caster->HasAura(MASTERY_SPELL_IGNITE) && l_Caster->getLevel() >= 80)
                        {
                            const SpellInfo *l_SpellInfo = sSpellMgr->GetSpellInfo(MASTERY_SPELL_IGNITE_AURA);
                            if (GetSpellInfo()->Id != MASTERY_SPELL_IGNITE_AURA && l_SpellInfo != nullptr)
                            {
                                float l_Value = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.5f;

                                int32 l_Bp = GetHitDamage();

                                if (l_Bp)
                                {
                                    l_Bp = int32(CalculatePct(l_Bp, l_Value));

                                    if (l_SpellInfo->Effects[EFFECT_0].Amplitude > 0)
                                        l_Bp = l_Bp / (l_SpellInfo->GetMaxDuration() / l_SpellInfo->Effects[EFFECT_0].Amplitude);
                                    
                                    if (l_Target->HasAura(MASTERY_SPELL_IGNITE_AURA, l_Caster->GetGUID()))
                                        l_Bp += l_Target->GetRemainingPeriodicAmount(l_Caster->GetGUID(), MASTERY_SPELL_IGNITE_AURA, SPELL_AURA_PERIODIC_DAMAGE);

                                    l_Caster->CastCustomSpell(l_Target, MASTERY_SPELL_IGNITE_AURA, &l_Bp, NULL, NULL, true);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_mastery_ignite_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_ignite_SpellScript();
        }
};

// Called by 35395 - Crusader Strike, 53595 - Hammer of the Righteous, 24275 - Hammer of Wrath, 85256 - Templar's Verdict, 53385 - Divine Storm, 157048 - Final Verdict
// 76672 - Mastery : Hand of Light
class spell_mastery_hand_of_light: public SpellScriptLoader
{
    public:
        spell_mastery_hand_of_light() : SpellScriptLoader("spell_mastery_hand_of_light") { }

        class spell_mastery_hand_of_light_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_hand_of_light_SpellScript);

            void HandleAfterHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster->GetTypeId() == TYPEID_PLAYER && l_Caster->HasAura(76672) && l_Caster->getLevel() >= 80)
                        {
                            uint32 l_ProcSpellId = GetSpellInfo()->Id ? GetSpellInfo()->Id : 0;
                            if (l_ProcSpellId != MASTERY_SPELL_HAND_OF_LIGHT)
                            {
                                int32 l_Bp = int32(CalculatePct(GetHitDamage(), l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.25f));
                                l_Caster->CastCustomSpell(l_Target, MASTERY_SPELL_HAND_OF_LIGHT, &l_Bp, NULL, NULL, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_mastery_hand_of_light_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_hand_of_light_SpellScript();
        }
};

// Called by 403 - Lightning Bolt, 421 - Chain Lightning, 51505 - Lava Burst and 117014 - Elemental Blast
// 77222 - Mastery : Elemental Overload
class spell_mastery_elemental_overload: public SpellScriptLoader
{
    public:
        spell_mastery_elemental_overload() : SpellScriptLoader("spell_mastery_elemental_overload") { }

        class spell_mastery_elemental_overload_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_elemental_overload_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(403) || !sSpellMgr->GetSpellInfo(421) || !sSpellMgr->GetSpellInfo(51505) || !sSpellMgr->GetSpellInfo(117014))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                SpellInfo const* procSpell = GetSpellInfo();

                if (procSpell)
                {
                    if (Unit* caster = GetCaster())
                    {
                        if (Unit* unitTarget = GetHitUnit())
                        {
                            // Lavaburst deals 50% more damage with Flame Shock on target
                            if (procSpell->Id == 51505 && unitTarget->HasAura(8050))
                                SetHitDamage(int32(float(GetHitDamage()) * 1.5f));

                            if (caster->GetTypeId() == TYPEID_PLAYER && caster->HasAura(77222))
                            {
                                // Every Lightning Bolt, Chain Lightning and Lava Burst spells have duplicate vs 75% damage and no cost
                                switch (procSpell->Id)
                                {
                                    // Lava Burst
                                    case 51505:
                                    {
                                        float Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;

                                        if (roll_chance_f(Mastery))
                                            caster->CastSpell(unitTarget, MASTERY_SPELL_LAVA_BURST, true);

                                        break;
                                    }
                                    // Lightning Bolt
                                    case 403:
                                    {
                                        float Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;

                                        if (roll_chance_f(Mastery))
                                            caster->CastSpell(unitTarget, MASTERY_SPELL_LIGHTNING_BOLT, true);

                                        break;
                                    }
                                    // Chain Lightning
                                    case 421:
                                    {
                                        float Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;

                                        if (roll_chance_f(Mastery))
                                            caster->CastSpell(unitTarget, MASTERY_SPELL_CHAIN_LIGHTNING, true);

                                        break;
                                    }
                                    // Elemental Blast
                                    case 117014:
                                    {
                                        float Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;

                                        if (roll_chance_f(Mastery))
                                        {
                                            caster->CastSpell(unitTarget, MASTERY_SPELL_ELEMENTAL_BLAST, true);
                                            caster->CastSpell(unitTarget, 118517, true); // Nature visual
                                            caster->CastSpell(unitTarget, 118515, true); // Frost visual
                                        }

                                        break;
                                    }
                                    default: break;
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mastery_elemental_overload_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_elemental_overload_SpellScript();
        }
};

// Call by Mortal Strike - 12294, Colossus Smash - 86346, Execute - 5308
// Mastery: Weapons Master - 76338
class spell_mastery_weapons_master : public SpellScriptLoader
{
public:
    spell_mastery_weapons_master() : SpellScriptLoader("spell_mastery_weapons_master") { }

    class spell_mastery_weapons_master_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_mastery_weapons_master_SpellScript);

        void HandleOnHit()
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->HasAura(SPELL_WARRIOR_WEAPONS_MASTER))
                {
                    float l_MasteryValue = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 3.5f;

                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_MasteryValue));
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_mastery_weapons_master_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_mastery_weapons_master_SpellScript();
    }
};

void AddSC_mastery_spell_scripts()
{
    new spell_mastery_weapons_master();
    new spell_mastery_sniper_training();
    new spell_mastery_recently_moved();
    new spell_mastery_sniper_training_aura();
    new spell_mastery_echo_of_light();
    new spell_mastery_icicles();
    new spell_mastery_icicles_trigger();
    new spell_mastery_icicles_periodic();
    new spell_mastery_icicles_hit();
    new spell_mastery_shield_discipline();
    new spell_mastery_blood_shield();
    new spell_mastery_ignite();
    new spell_mastery_hand_of_light();
    new spell_mastery_elemental_overload();
}