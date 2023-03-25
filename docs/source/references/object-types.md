---
hide:
  - toc
---

# Object Types

!!! tip
	These values are available in Lua by their index in the `tes3.objectType` table. For example, `tes3.objectType.creature` has a value of `1095062083`.

!!! note
	Their corresponding construction set ID (if available) and description are listed as well, but are not available through the `tes3.objectType.*` namespace.

!!! note
	The **AMMO** object type is for arrows and bolts only, even though these usually show up in the weapons tab of the Construction Set. Thrown weapons do not have this type.

Index                 | Value        | CS ID   | Description             | MWSE Type
--------------------- | ------------ | ------- | ----------------------- | ---------
activator             | `1230259009` | ACTI    | Activator               | tes3activator
alchemy               | `1212369985` | ALCH    | Alchemy                 | tes3alchemy
ammunition            | `1330466113` | AMMO    | Ammunition              | tes3weapon
apparatus             | `1095782465` | APPA    | Apparatus               | tes3apparatus
armor                 | `1330467393` | ARMO    | Armor                   | tes3armor
birthsign             | `1313297218` | BSGN    | Birthsign               | tes3birthsign
bodyPart              | `1497648962` | BODY    | Body Part               | tes3bodyPart
book                  | `1263488834` | BOOK    | Book                    | tes3book
cell                  | `1280066883` | CELL    | Cell                    | tes3cell
class                 | `1396788291` | CLAS    | Class                   | tes3class
clothing              | `1414483011` | CLOT    | Clothing                | tes3clothing
container             | `1414418243` | CONT    | Container               | tes3container
creature              | `1095062083` | CREA    | Creature                | tes3creature
dialogue              | `1279347012` | DIAL    | Dialogue Topic          | tes3dialogue
dialogueInfo          | `1330007625` | INFO    | Dialogue Response       | tes3dialogueInfo
door                  | `1380929348` | DOOR    | Door                    | tes3door
enchantment           | `1212370501` | ENCH    | Enchantment             | tes3enchantment
faction               | `1413693766` | FACT    | Faction                 | tes3faction
gmst                  | `1414745415` | GMST    | Game Setting            | tes3gameSetting
ingredient            | `1380404809` | INGR    | Ingredient              | tes3ingredient
land                  | `1145979212` | LAND    | Land                    | tes3land
landTexture           | `1480938572` | LTEX    | Land Texture            | tes3landTexture
leveledCreature       | `1129727308` | LEVC    | Leveled Creature        | tes3leveledCreature
leveledItem           | `1230390604` | LEVI    | Leveled Item            | tes3leveledItem
light                 | `1212631372` | LIGH    | Light                   | tes3light
lockpick              | `1262702412` | LOCK    | Lockpick                | tes3lockpick
magicEffect           | `1178945357` | MGEF    | Magic Effect            | tes3magicEffect
miscItem              | `1129531725` | MISC    | Misc Item               | tes3misc
mobileActor           | `1413693773` | MACT    | Mobile Actor            | tes3mobileActor?
mobileCreature        | `1380139341` | MACR    | Mobile Creature         | tes3mobileCreature
mobileNPC             | `1212367181` | MACH    | Mobile NPC              | tes3mobileNPC
mobilePlayer          | `1346584909` | MACP    | Mobile Player           | tes3mobilePlayer
mobileProjectile      | `1246908493` | MPRJ    | Mobile Projectile       | tes3mobileProjectile
mobileSpellProjectile | `1347637325` | MPSP    | Mobile Spell Projectile | tes3mobileSpellProjectile
npc                   | `1598246990` | NPC_    | NPC                     | tes3npc or tes3npcInstance
pathGrid              | `1146242896` | PGRD    | Pathgrid                | tes3pathGrid
probe                 | `1112494672` | PROB    | Probe                   | tes3probe
quest                 | `1397052753` | QUES    | Quest                   | tes3quest
race                  | `1162035538` | RACE    | Race                    | tes3race
reference             | `1380336978` | REFR    | Reference               | tes3reference
region                | `1313293650` | REGN    | Region                  | tes3region
repairItem            | `1095779666` | REPA    | Repair Item             | tes3repairTool
script                | `1414546259` | SCPT    | Script                  | tes3script
skill                 | `1279871827` | SKIL    | Skill                   | tes3skill
sound                 | `1314213715` | SOUN    | Sound                   | tes3sound
soundGenerator        | `1195658835` | SNDG    | Sound Generator         | tes3soundGenerator
spell                 | `1279610963` | SPEL    | Spell                   | tes3spell
startScript           | `1380143955` | SSCR    | Start Script            | tes3startScript
static                | `1413567571` | STAT    | Static                  | tes3static
weapon                | `1346454871` | WEAP    | Weapon                  | tes3weapon
