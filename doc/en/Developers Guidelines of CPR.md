#  Developer Guidelines of Combat Pathing Revolution
This article would instructs you on how to utilize **Combat Pathing Revolution** (refers as **CPR**) to produce customizable combat AI pathing data for npcs melee combat. Article comes with several sections below:
* [**Descriptions of Variables**](#descriptions-of-variables)
* [**Debugging In Game**](#debugging-in-game)
* [**Methods for Implementing**](#methods-for-implementing)

<br/> 

## Descriptions of Variables
---  
In order to implement a dynamic and highly customizable AI pathing feature, CPR using graph variables (A.K.A [Animation Variables](https://www.creationkit.com/index.php?title=List_of_Animation_Variables)) to represents the AI data that using to control the melee combat pathing AI for npc during the combat gameplay. CPR using these graph variables to override those core data that controled the AI pathing, therefore with CPR, each npc in game now could has a completely unique pathing dataset as it owned which is a great different to the vanilla "single global value assign to everyone".  

One thing have to mention before we go detail is that **you don't need to consider the weapon length distance when assiging all of those CPR distance variables**, the weapon length will be added automactilly from skse plugin end when computing the final result.

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
NPC could perform circling movement around its combat target during close range combat. CPR has introduced min and max distance restriction for circling, in order to make combat circling acts more like the way in modern action game:
* **CPR_EnableCircling**: boolean variable, set the value to true to enable the override of vanilla combat circling data with CPR data, reset the value to false to keep everything working as vanilla.
* **CPR_CirclingDistMin**: float variable, its value represents the necessary minimum distance between npc to it current combat target that allow to perform circling movement. Only when the distance to the combat target is greater than this value, NPC would intend to perform circling. The variable value should be greater than **CPR_InnerRadiusMin** to work as intended.
*  **CPR_CirclingDistMax**: float variable, its value represents the maximum distance between npc to it current combat target that allow to perform circling movement. Only when the distance to the combat target is less than the value, NPC would intend to perform circling. The variable value should be greater than **CPR_CirclingDistMin**.
*  **CPR_CirclingAngleMin** float variable, its value represents the minimum angle the npc would moving around it current combat target when perform circling movement. The variable value should be greater than zero and less than 180.
*  **CPR_CirclingAngleMax** float variable, its value represents the maximum angle the npc would moving around it current combat target when perform circling movement. The variable value should be greater than **CPR_CirclingAngleMin** and less than 180.
  
Be caution that npc won't perform circling if it has not **Close Range - Dueling** box checked in its combat style.  

<br/>   

### Fallback
NPC could perform fallback during close range combat, how far the distance the npc would moving during a fallback is decided by its combat style fallback mult and CPR graph variables below:  
* **CPR_EnableFallback**: boolean variable, set the value to true to enable the override of vanilla combat fallback data with CPR data, reset the value to false to keep everything working as vanilla.
*  **CPR_FallbackDistMin**: float variable, its value represents the minimum fallback distance that the actor identify during fallback. The variable value should be at least greater than **CPR_InnerRadiusMid** to work as intended.
*  **CPR_FallbackDistMax**: float variable, its value represents the maximum fallback distance that the actor identify during fallback. The variable value should be at least greater than **CPR_FallbackDistMin** to work as intended.  
The formula to comput the actual fallback distance in game is as following:  
```
FallbackDistance = CPR_FallbackDistMin + (CPR_FallbackDistMax - CPR_FallbackDistMin) * CombatStyleFallbackMult 
``` 

*  **CPR_FallbackWaitTimeMin**: float variable, its value represents the minimum waiting time the npc would  waits in place after performed a fallback. ThIS variable value should be at least greater than zero to work as intended.
*  **CPR_FallbackWaitTimeMax**: float variable, its value represents the maximum waiting time the npc would  waits in place after performed a fallback. ThIS variable value should be at least greater than **CPR_FallbackWaitTimeMin** to work as intended.
The formula to comput the actual fallback wait time in game is as following:  
```
FallbackWaitTime = CPR_FallbackWaitTimeMin + (CPR_FallbackWaitTimeMax - CPR_FallbackWaitTimeMin) * CombatStyleFallbackMult 
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
<br/>   

## Methods for Implementing
---
So after debugged in game you now have a "MaxsuCPR_Enable.ini" file that stored the CPR pathing data for the npc, next step you need to apply these CPR data as a playable content into the game.
Below are the practical methods you can use to do that:<br/>  
  
### Via animation annotations:  
Using annotations is a suitable method to implement if your CPR pathing dataset is designed for a specific attack moveset. There are five instructions that can be annotated to set up CPR data for an actor:  
1. Active the advance data on the actor:<br/>
   `CPR.EnableAdvance|(float)InnerRadiusMin|(float)InnerRadiusMid|(float)InnerRadiusMax|(float)OuterRadiusMin|(float)OuterRadiusMid|(float)OuterRadiusMax`<br/> 
    These console commands in "MaxsuCPR_Enable.ini":
    ```
    SetAnimGraphVar CPR_EnableAdvanceRadius  1

    SetAnimGraphVar CPR_InnerRadiusMin 90

    SetAnimGraphVar CPR_InnerRadiusMid 110    

    SetAnimGraphVar CPR_InnerRadiusMax 300

    SetAnimGraphVar CPR_OuterRadiusMin 300

    SetAnimGraphVar CPR_OuterRadiusMid 500

    SetAnimGraphVar CPR_OuterRadiusMax 1000
    ```
    would be equal to annotation:<br/>
    `CPR.EnableAdvance|90|110|300|300|300|500|1000|`  
    <br/>      

2. Active the backoff data on the actor:<br/>
   `CPR.EnableBackoff|(float)BackoffMinDistMult|(float)CPR_BackoffChance`<br/> 
    These console commands in "MaxsuCPR_Enable.ini":
    ```
    SetAnimGraphVar CPR_EnableBackoff  1

    SetAnimGraphVar CPR_BackoffMinDistMult 0.85

    SetAnimGraphVar CPR_BackoffChance 0.95
    ```
    would be equal to annotation:<br/> 
    `CPR.EnableBackoff|0.85|0.95`  
    <br/> 

3. Active the circling data on the actor:<br/>
   `CPR.EnableCircling|(float)CirclingDistMin|(float)CirclingDistMax|(float)CirclingAngleMin|(float)CirclingAngleMax`<br/> 
    These console commands in "MaxsuCPR_Enable.ini":
    ```
    SetAnimGraphVar CPR_EnableCircling 1

    SetAnimGraphVar CPR_CirclingDistMin 90

    SetAnimGraphVar CPR_CirclingDistMax 1200

    SetAnimGraphVar  CPR_CirclingAngleMin 90

    SetAnimGraphVar  CPR_CirclingAngleMax 120
    ```
    would be equal to annotation:<br/> 
    `CPR.EnableCircling|90|1200|90|120`  
    <br/> 

4. Active the fallback data on the actor:<br/>
   `CPR.EnableFallback|(float)FallbackDistMin|(float)FallbackDistMax|(float)FallbackWaitTimeMin|(float)FallbackWaitTimeMax`<br/> 
   These console commands in "MaxsuCPR_Enable.ini":
    ```
    SetAnimGraphVar CPR_EnableFallback 1

    SetAnimGraphVar CPR_FallbackDistMin 200

    SetAnimGraphVar CPR_FallbackDistMax 500

    SetAnimGraphVar CPR_FallbackWaitTimeMin 2.0

    SetAnimGraphVar CPR_FallbackWaitTimeMax 3.0
    ```
    would be equal to annotation:<br/> 
    `CPR.EnableFallback|200|500|2.0|3.0`  
    <br/>

5. Disable all the CPR data on the actor and restore everything back to vanilla:<br/>
    `CPR.DisableAll`<br/>
    This annotation would be equal to console commands:
    ```
    SetAnimGraphVar CPR_EnableAdvanceRadius 0

    SetAnimGraphVar CPR_EnableBackoff  0

    SetAnimGraphVar CPR_EnableCircling 0

    SetAnimGraphVar	CPR_EnableFallback 0
    ```

Animations that could be used to active or disable these CPR instructions for a moveset are those weapon draw/sheath animations. The "_CPR Patched Animations Pack_" I patched did so. Here are the name of those weapon draw/sheath animations you can use for:
```
1hm_equip.hkx - One handed sword draw
1hm_unequip.hkx - One handed sword sheath
dag_equip.hkx - Dagger draw
dag_unequip.hkx - Dagger sheath
axe_equip.hkx - One handed axe draw
axe_unequip.hkx - One handed axe sheath
mac_equip.hkx - Mace draw
mac_unequip.hkx - Mace sheath
h2h_equip.hkx - Fits draw
h2h_unequip.hkx - Fits sheath
2hc_equip.hkx - Two handed sword draw
2hc_unequip.hkx - Two handed sword sheath
2hw_equip.hkx - Two handed warhammer/battleaxe draw
2hw_unequip.hkx - Two handed warhammer/battleaxe sheath
```

Simply annotate your `CPR.EnableAdvance`, `CPR.EnableBackoff`, `CPR.EnableCircling`, `CPR.EnableFallback` instructions inside the weapon equip hkx file, while annotate the `CPR.DisableAll` inside the wepon unequip hkx file.
<br/><br/>   

### Via Papyrus Scrips:
**To Be Done** 

---