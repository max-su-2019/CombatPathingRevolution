#  Developer Guidelines of Combat Pathing Revolution
This article would instructs you on how to utilize **Combat Pathing Revolution** (refers as **CPR**) to produce customizable combat AI pathing features for npcs melee combat. Article comes with several sections below:
* [**Descriptions of Variables**](#descriptions-of-variables)
* [**Debugging In Game**](#debugging-in-game)
* **Methods for Implementing**

<br/> 

## Descriptions of Variables
---  
In order to implement a dynamic and highly customizable AI pathing feature, CPR using graph variables (aslo called [Animation Variables](https://www.creationkit.com/index.php?title=List_of_Animation_Variables)) to represent the core data that relates to the melee combat pathing control for an npc in the game engine. CPR using those graph variables to override those core data that controled the AI pathing, therefore with CPR, each npc in game now could has a completely unique pathing dataset that different from the vanilla one global value assign to everyone design.  

One thing to mention before everthing is that **you don't need to consider the weapon length distance when assiging all of those CPR distance variables**, the weapon length will be added automactilly from skse plugin end when computing the final result.

Below are the actions that NPC would perform during combat pathing and the CPR graph variables could be used to control these actions:

### Advance
NPC would sprinting forward to approach the current combat target if distance to the target is beyond its **combat advance outer radius**, while the distance is between the **combat advance outer radius** and **combat advance inner radius**, npc would then advances in normal speed to approach the target.
Here are the graph variables that can be used to control this action:
* **CPR_EnableAdvanceRadius**: boolean variable, set the value to true to enable the override of vanilla combat advance radius data with CPR data, reset the value to false to keep everything working as vanilla.
*  **CPR_InnerRadiusMin**: float variable, its value represents the minimum inner radius value that the actor identify during pathing. The variable value should be greater than zero.
*  **CPR_InnerRadiusMid**: float variable,this value represents the medium inner radius value that the actor identify during pathing.  The variable value should be greater than **CPR_InnerRadiusMin**.
*  **CPR_InnerRadiusMax**: float variable, its value represents the maxnium inner radius value that the actor identify during pathing. The variable value should be greater than **CPR_InnerRadiusMid**.
*  **CPR_OuterRadiusMin**: float variable, it value represents the minimum inner radius value that the actor identify during pathing. The variable value should be greater than **CPR_InnerRadiusMin**.
*  **CPR_OuterRadiusMid**: float variable, it value represents the medimum inner radius value that the actor identify during pathing. The variable value should be greater than **CPR_InnerRadiusMid** and **CPR_OuterRadiusMin**.
*  **CPR_OuterRadiusMax**: float variable, it value represents the maximum inner radius value that the actor identify during pathing. The variable value should be greater than **CPR_InnerRadiusMax** and **CPR_OuterRadiusMid**.  

The formula to get the real combat advance radius value is as following:

* If NPC want make offensive:
```
InnerRadius = CPR_InnerRadiusMin + (CPR_InnerRadiusMid - CPR_InnerRadiusMin) * (1 - offensiveRatio)

OuterRadius = CPR_OuterRadiusMin + (CPR_OuterRadiusMid - CPR_OuterRadiusMin) * (1 - offensiveRatio)
```

* If NPC is defensive:
```
InnerRadius = CPR_InnerRadiusMid + (CPR_InnerRadiusMax - CPR_InnerRadiusMid) * defensiveRatio

OuterRadius = CPR_OuterRadiusMid + (CPR_OuterRadiusMax - CPR_OuterRadiusMid) * defensiveRatio
```  
<br/> 

### Backoff
Npc would perform back off(the short distance retreating) when it consider itself too close to it combat target. CPR introduced these graph variables to provide more customized in the backoff action:
* **CPR_EnableBackoff**: boolean variable, set the value to true to enable the override of vanilla backoff data with CPR data, reset the value to false to keep everything working as vanilla.
* **CPR_BackoffMinDistMult**: float variable, Backoff would only trigger when:  
`combatTargetDistance < InnerRadius * CPR_BackoffMinDistMult`

* **CPR_BackoffChance**: float variable, it value represents the possibility that npc performs backoff.  

<br/> 

### Circling
NPC could perform circling around its combat target during close range combat. CPR has introduced min and max distance restriction for circling, in order to make combat circling acts more like the way in modern action game:
* **CPR_EnableCircling**: boolean variable, set the value to true to enable the override of vanilla combat circling data with CPR data, reset the value to false to keep everything working as vanilla.
* **CPR_CirclingDistMin**: float variable, its value represents the necessary minimum distance from the combat target to perform circling. Only when the distance to the combat target is greater than the value, NPC would intend to perform circling. The variable value should be greater than **CPR_InnerRadiusMin** to work as intended.
*  **CPR_CirclingDistMax**: float variable, its value represents the maximum distance from the combat target to perform circling. Only when the distance to the combat target is less than the value, NPC would intend to perform circling. The variable value should be greater than **CPR_CirclingDistMin**.  
  
Be caution that npc would won't perform circling if it has not **Close Range - Dueling** box checked in its combat style.  

<br/>   

### Fallback
NPC could perform fallback during close range combat, how far the distance the npc would moving during a fallback is decided by its combat style fallback mult and CPR graph variables below:  
* **CPR_EnableFallback**: boolean variable, set the value to true to enable the override of vanilla combat fallback data with CPR data, reset the value to false to keep everything working as vanilla.
*  **CPR_FallbackDistMin**: float variable, its value represents the minimum fallback distance that the actor identify during fallback. The variable value should be at least greater than **CPR_InnerRadiusMid** to work as intended.
*  **CPR_FallbackDistMax**: float variable, its value represents the minimum fallback distance that the actor identify during fallback. The variable value should be at least greater than **CPR_FallbackDistMin** to work as intended.

The formula to comput the real fallback distance is as following:
```
FallbackDistance = CPR_FallbackDistMin + (CPR_FallbackDistMax - CPR_FallbackDistMin) * CombatStyleFallbackMult 
```

Be caution that npc would won't perform fallback if it has not **Close Range - Dueling** box checked in its combat style.  

---
<br/> 

## Debugging In Game
---
A convenience way to set and test CPR pathing data for npc during gameplay is by using [console command batch files](https://skyrimcommands.com/command/bat). CPR has provided three command batch files to satisfy the demand of debugging:
* "MaxsuCPR_Enable.ini": Run this batch file would enable and set up CPR pathing data for an npc, and override its vanilla combat pathing data with CPR graph variable.
*  "MaxsuCPR_Disable.ini": Run this batch file would disable CPR pathing data effect on an npc, and restore its combat pathing back to vanilla.
*  "MaxsuCPR_Display.ini": Run this batch file would display all of the CPR graph variables value on the console screen.

To run the batch file, simply selcet an npc in the console then enter command like this:
```
bat "MaxsuCPR_Enable.ini"
```
