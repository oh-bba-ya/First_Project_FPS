# First_Project_FPS
A third-person shooter lateral scroll game based on Unreal Engine. It is a project based on Metal Slug.

![image](https://user-images.githubusercontent.com/121008986/212522772-20b9fa4c-4a09-4a7f-ad40-fdde30517eb9.png)

* The project was produced based on the first stage of Metal Slug, and the game will be finished when the first boss-class monster, helicopter, have eliminate.

# Game logic and functionality
![image](https://user-images.githubusercontent.com/121008986/212522372-b6fcd99c-3421-49cb-99d1-2029ccd18664.png)
* Player's HP on the top left corner
* Below it is marked the number of ammunition and bombs the player has.
* Below that is the player's current score.

![image](https://user-images.githubusercontent.com/121008986/212522427-a37c5f4d-9bb9-442a-af69-22e8173e792b.png)
* Overlap with the NPC in front of the player creates an item that replenishes ammunition and is implemented to destroy itself.

![image](https://user-images.githubusercontent.com/121008986/212522498-10445e9e-4b79-4833-9216-00b09ea94f83.png)
* The generated item replenishes the matching ammunition according to the gun you are currently holding.
* There are two types of guns: a sniper gun with linetrace and a grand gun that fires bombs.

![image](https://user-images.githubusercontent.com/121008986/212522511-125ffae6-cec6-4b52-9c36-9babfefc8126.png)
* Enemy AI was implemented using pawnsensing, and this enemy has the function of recognizing and tracking the player's location by using front view or capturing the sound made by the player.
![image](https://user-images.githubusercontent.com/121008986/212523033-a3c67c06-108e-4dfc-b382-ccb04daffddc.png)
* The Enemy AI is blue-printed.
* A Triggerbox was placed so that when enemy stepped on it, the player could be tracked even on separate terrain by jumping.

![image](https://user-images.githubusercontent.com/121008986/212522629-183f5309-1a54-4176-8548-2f86027b8e26.png)
* The Trigger Box was placed in a specific part and configured to summon the boss to the front when overlapped.

![image](https://user-images.githubusercontent.com/121008986/212522645-a8cd2db7-34f0-4fd7-bc62-d1e90883fbe0.png)
* The recalled boss recognizes and tracks the player's location and demonstrates a pattern of dropping three bombs when it reaches the top of the player. And of course, the player's HP decreases when hit by this bomb.

![image](https://user-images.githubusercontent.com/121008986/212522717-686063c4-c0c3-4b5a-983c-ceef409e31c9.png)
* Since the boss flies in the air, it has added a function to allow the gun to be pointed upward to be suitable for dealing with it. Through the upward key, the gun can be pointed upward to damage the boss through a bullet.
* If the boss's HP is damaged as much as it is set, the ending UI will appear on the screen, and the game will be played again in the form of ending the game through the Quit button or recalling the Level through the Restart button.

# Level Design
 * The corresponding level is made to resemble a metal slug map by utilizing meshes from Quixel and Epic Marketplace.
 
# Kinect Details
 * Tools and Platform:
  ** Unreal Engine 5.1
  ** Visual Studio 2019
  
 * Shooting：
  ** The method of generating bullets from the front of the Player Character was adopted, and Enemy's bullets were set to be fired in that direction after locating the player on the map through the Iterator function.
  ** Sniper guns were manufactured using the Linetrace method and were designed to recognize and damage enemies located in a straight line.


