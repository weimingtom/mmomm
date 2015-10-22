# Basic Stats #

Monsters (and creatures in general) should have a few inherent stats that can be modified by abilities and so forth. We could make stuff like walking or having health an ability attached to the monster or something, I guess, but I think this makes more sense.

  * Health
  * Health regeneration (per some suitable interval)
  * Energy
  * Energy regeneration (per some suitable interval)
  * Move Speed
  * Movement type or something if we have like water monsters

# Abilities #

Abilities provide a generic framework for creature/monster innate powers, magic, and equipment. Examples will be provided to clarify things where possible, although the specific monsters, abilities, and so forth aren't necessarily intended to be in the game.

An ability always has at least one of each of the following: activation type; target; duration; effect.

## Activation Types ##

There are two major categories: passive and active. This distinction need not necessarily be made in code, although it may be desirable for the interface. Creatures can decide when and whether to use an active ability, while passive abilities are constant or activated by conditions.

Passive ability activations should ideally let the rest of the ability code access activation-related data that seems relevant (e.g. for "when creature collides with something", what the creature collided with).

I don't think a single ability should need to have multiple activation types, but support for it wouldn't hurt.

### Passive Ability Activation Types ###

Not necessarily a totally exhaustive list, but should be a solid base. Care should be taken regarding the timing of abilities and things like damage or death to avoid misunderstandings/bugs.

  * While held, ability is active (like "health regeneration" for a troll or "damage reduction" for a suit of armor) - I'm not completely set on how to handle this one
  * Every X time intervals, ability activates
  * When creature is damaged, ability activates
  * When creature dies, ability activates
  * When creature deals damage, ability activates
  * When creature kills something, ability activates
  * When creature collides with something, ability activates
  * When creature is struck by a projectile, ability activates
  * When creature's health or energy reaches a certain value, ability activates

### Active Ability Activation Types ###

  * Creature chooses to use ability (and use of ability is currently allowed)

## Targets ##

Combinations of targets should be accepted (like "self and first creature struck by projectile"). There should _maybe_ be a flag for whether it only hits allies/enemies, but I'm not sure on that matter.

There should be a flag for whether targeting allows self. Might save computational time to have a special "self" target, though, instead of using nearest creature.

There should probably be further specifications for particular qualities the target must have, like 'be a tree'?

If an ability is passive and no suitable target is available, the ability's activation should be ignored/cancelled. Invalid targets for active abilities (say, from a hacked client) should also be rejected.

  * Nearest creature
  * All creatures within X distance
  * Targeted creature within X distance
  * All creatures in X radius around a targeted point within Y distance
  * First creature struck by projectile(s) (note that various projectile behaviors should be possible; shouldn't be assumed simply to shoot in a straight line)
  * All creatures struck by projectile(s)
  * Location within X distance
  * Creature within X distance, and location within Y distance of self and Z distance of targeted creature (or somesuch; this is for teleports etc)

## Sources ##

Of course, an ability has to come from somewhere -- the eyes, an aura, etc.

  * Aura
  * Gaze
  * Spitting
  * Breathing
  * Engulfing

## Duration ##

An ability's effect can be instantaneous (applied once, usually healing/damage), have a duration expressed in some time interval, or be permanent (unless dispelled or whatever).

## Effects ##

Abilities can have any number of effects and can have the same base effect multiple times (like "10 health damage (fire); 5 health damage (ice)").

  * Health damage (all damage effects can carry tag(s) like "fire") (instantaneous works as expected, duration means the damage goes away at the end)
  * Ticked health damage (deals X damage every Y time intervals for duration)
  * Energy damage
  * Ticked energy damage
  * Health regeneration change (regen cannot go below 0, ticked damage can be used for that)
  * Energy regeneration change
  * Health increase (instantaneous = healing, duration means it goes away at the end like Fortify Health in Morrowind)
  * Energy increase (ditto)
  * Absorb health / absorb energy (should take reduction etc into account)
  * Reduce received health damage by X minimum Y (can be global or specify particular tag(s) to exclude or solely affect)
  * Reduce received health damage by X%
  * Reduce received energy damage by X minimum Y
  * Reduce received energy damage by X%
  * Reduce outgoing damage by X minimum Y (again, can be global or exclude/specify tag(s))
  * Reduce outgoing damage by X% minimum Y
  * Move speed change
  * Movement type change
  * Summon creature
  * Move creature to point
  * Conceal/invisibility
  * Disable object activation (for like a spell that targets doors and "locks" them)
  * Dispel active spell effects (with some kind of check?)
  * Add ability (gets removed when duration (of parent) is up)

## Cost / Cooldown ##

Abilities can also have certain conditions that must be satisfied before they can be cast, and can have costs that cut through energy damage reduction or whatever. For example, a typical spell might require 5 energy to cast and remove 5 energy from the caster. If the conditions are not satisfied, the spell does not go off (even if passive). As a particularly notable condition, abilities can have a minimum cooldown specified between uses.

## Sample applications ##

(Numbers are totally arbitrary.)

"Druid's Wall" - active; targets all trees within 80 distance of selected point; duration 60 seconds; adds Ironbark ability (Ironbark: passive while held; self; permanent; reduce health damage by 3 and reduce health damage from (fire) tag by 50% ); costs 5 energy to cast.

"Aura of Healing" - passive, activates every 3 seconds; targets all creatures within 30 distance; duration 3 seconds; health regeneration +5/second.