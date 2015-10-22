# Equipment Slots #

Each character has eight equipment slots:
  * Helmet
  * Chest
  * Gloves
  * Boots
  * Main hand
  * Offhand
  * Accessory #1
  * Accessory #2

Accessories include amulets, rings, girdles, etc.

Each player gets 50 inventory slots.

# Item Attributes #

An item has a quality between 0 and 100. The player sees generic quality indicators: bad, poor, mediocre, good, excellent, masterful.

Can items be damaged? I'm not sure we decided yet.

Each item can be a stack of up to 99 similar items, based on outward appearance. All items are stored as distinct entities.

# Material #

Materials that an item could be made out of include:
  * Metal
  * Wood
  * Rock
  * Clay
  * Glass
  * Cloth
  * Leather
  * Bone
  * Meat
  * Vegetable
  * Wax
  * Paper
  * Liquid
  * Vapor
  * Light

We may wish to break down Metal, Wood and Rock into further types.

Materials affect the attributes of an item. If an item is created out of two types of material, the more relevant (the axe head rather than the shaft) is used for the type of the result, but the components figure into attributes such as weight and quality (a high-quality wood axe head would make a pretty bad axe).

# Tech Tree #

The player can make new items by either applying a tool to an existing item or by assembling two items.

## Real-World Smelting ##

From Wikipedia.

Smelting involves turning Oxides and Sulfides into pure metals and getting rid of slag.

  1. To get rid of slag, you add flux, like lime.
  1. Sulfides are converted to oxides by roasting; burning wood on top of the ore. (2ZnS + 3O<sub>2</sub> -> 2ZnO + 2SO<sub>2</sub>)
  1. Oxides are smelted with carbon in the form of charcoal. (ZnO + CO -> Zn + CO<sub>2</sub>)

**Gold** is the oldest metal; often found in as flakes, grains, or nuggets in placer deposits. Was best harvested with hydraulic mining; found mostly pure and could be collected by filtering out the sediment with water. No smelting necessary.

**Silver** ore (Ag<sub>2</sub>S) is mainly found in a **Lead** ore, Galena (PbS). Gold also contains a small percentage of silver, but there was no method to extract it. Electrum is gold with high silver content (>10%).

**Copper** ores are some of the oldest used (3000 BC). It can be found as native copper, or as ores like malachite or azurite. It was forged with zinc, tin or arsenic ore to produce a brass or bronze.

**Steel** has been forged as long as iron, dating back to 2000 BC. It was first mass-produced in China, then Damascus, about 300 BC using a very complicated method which has been lost because the raw materials used to produce it ran out.

In Europe in medieval to modern times (1600-1850), steel was produced by making pig iron from a very pure Swedish iron ore, fining it to wrought iron with charcoal, then cementing it with carbon for about a week (sorta like firing it in a kiln).

**Aluminum** was shown to exist in 1808, and the process to produce it was not discovered until much later.

## Metals ##

Some ideas: Gold, silver, iron, tin, copper, mercury, lead, zinc, orichalcum, unobtainium, adamantium, mithril.

It is impossible to decompose metals, ores are almost never pure, and it is impossible to tell how pure an ore is. You can, however, mix metals to get alloys.

Forge ores to get bars of 75-100% purity, in terms of other metals, with a quality rating indicating what part of the bar is not slag. Anything less than 75% pure is called an alloy, such as "copper alloy." Any component with greater than 25% composition is named, "copper-iron alloy"; some pairs have names, like "bronze." Any metal with no component greater than 25% is called "scrap" and given terrible stats.

You can examine aspects of a metal bar to tell more about it. Immediately apparent are the name and the computed color of the ore, which give some hint as to its composition. Also apparent is its quality rating, which gives some indication of the slag:metal ratio. Tools are needed to examine the other attributes, which may include: ductility, tensile/shear strength, melting point, resistance to rust/corrosion, conductivity, and mass/density. These will be used to compute the final product's quality and weight and perhaps other properties. Tools used to test these properties will be split, according to tool quality between 2 and 8 subtypes.

For the tools, it might be efficient to have a workbench mode where you split a stack of material according to its quality, then restack and split further according to another property.

## Wood/Plants ##

Trees and plants can be found in the wild, often living in environments that suit them because otherwise the previous generation would not have grown to produce them. They can be farmed more efficiently, but often less potently, as the world should be sufficiently volatile so that players have difficulty creating controlled environments.

Conditions which the plant does not like will affect the plant's reproduction as well as its yield. Yew trees which are starved of water for a period of time may die, but some would also get an extra resilience, or have less water content and thus create better charcoal.

## Stone ##

Stone can be tested similarly to wood, except that stone is not created, but found. Generic type-name, quality and color help the player identify the base components. Tools can be used to determine other properties.

