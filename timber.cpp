#include "unsigned_char_traits.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
using namespace sf;
using namespace std;
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side
{
  LEFT,
  RIGHT,
  NONE
};
side branchPositions[NUM_BRANCHES];
int main()
{
  RenderWindow window(VideoMode(960, 540), "Timber");
  window.setFramerateLimit(60);
  View view(FloatRect(0, 0, 1920, 1080));
  window.setView(view);
  Clock clock;
  // Making the Backround
  Texture textureBackground;
  textureBackground.loadFromFile("graphics/background.png");
  Sprite spriteBackground(textureBackground);
  spriteBackground.setPosition(0, 0);

  // Making the tree Sprite
  Texture textureTree;
  textureTree.loadFromFile("graphics/tree.png");
  Sprite spriteTree(textureTree);
  spriteTree.setPosition(810, 0);
  // Making the Bee
  Texture textureBee;
  textureBee.loadFromFile("graphics/bee.png");
  Sprite spriteBee(textureBee);
  spriteBee.setPosition(0, 800);
  bool beeActive = false;
  float beeSpeed = 0.0f;

  // Making the clouds
  Texture textureCloud;
  textureCloud.loadFromFile("graphics/cloud.png");
  Sprite spriteCloud1(textureCloud);
  Sprite spriteCloud2(textureCloud);
  Sprite spriteCloud3(textureCloud);
  spriteCloud1.setPosition(0, 0);
  spriteCloud2.setPosition(0, 250);
  spriteCloud3.setPosition(0, 500);
  bool cloud1Active = false;
  bool cloud2Active = false;
  bool cloud3Active = false;
  float cloud1Speed = 0.0f;
  float cloud2Speed = 0.0f;
  float cloud3Speed = 0.0f;

  bool pause = true;

  // Implementing the HUD
  int score = 0;
  Text scoreText;
  Text messageText;
  Font font;
  font.loadFromFile("fonts/KOMIKAP_.ttf");
  scoreText.setFont(font);
  messageText.setFont(font);
  messageText.setString("Press Enter to start!");
  scoreText.setString("Score = 0");
  messageText.setCharacterSize(75);
  scoreText.setCharacterSize(100);
  messageText.setFillColor(Color::White);
  scoreText.setFillColor(Color::White);

  FloatRect textRect = messageText.getGlobalBounds();
  // messageText.setOrigin(textRect.left+textRect.width/2.0f,textRect.top+textRect.height/2.0f);
  // messageText.setPosition(1920/2.0f,1080/2.0f);
  messageText.setPosition(1920 / 2.0f - (textRect.width / 2), 1080 / 2.0f - (textRect.height / 2));

  scoreText.setPosition(20, 20);

  // Implementing the time bar
  RectangleShape timeBar;
  float barWidth = 400;
  float barHeight = 80;
  Vector2f barDim(barWidth, barHeight);
  timeBar.setSize(barDim);
  timeBar.setPosition((1920 / 2) - barWidth / 2, 980);
  timeBar.setFillColor(Color::Red);
  Time totalgameTime;
  float timeRemaining = 6.0f;
  float barWidthPerSecond = barWidth / timeRemaining;

  // Preparing 6 branches
  Texture textureBranch;
  textureBranch.loadFromFile("graphics/branch.png");
  for (int i = 0; i < NUM_BRANCHES; i++)
  {
    branches[i].setTexture(textureBranch);
    branches[i].setPosition(-2000, -2000);
    branches[i].setOrigin(220, 20);
  }

  

  //Preparing the player
  Texture texturePlayer;
  texturePlayer.loadFromFile("graphics/skeleton.png");
  Sprite spritePlayer(texturePlayer);
  spritePlayer.setPosition(580,720);
  side playerSide=side::LEFT;//player will start from left

  //Prepare the gravestone
  Texture textureRIP;
  textureRIP.loadFromFile("graphics/rip.png");
  Sprite spriteRIP(textureRIP);
  spriteRIP.setPosition(600,860);

  //Prepare the axe
  Texture textureAxe;
  textureAxe.loadFromFile("graphics/axe.png");
  Sprite spriteAxe(textureAxe);
  spriteAxe.setPosition(700,830);
  //Lining up the axe wrt tree
  const float AXE_POSITION_LEFT=700;
  const float AXE_POSITION_RIGHT=1075;
  
  //Making the log
Texture textureLog;
textureLog.loadFromFile("graphics/log.png");
Sprite spriteLog;
spriteLog.setTexture(textureLog);
spriteLog.setPosition(810,720);

bool logActive=false;
float logSpeedX=1000;
float logSpeedY=-1500;
  
bool acceptInput=false;//control player input
// Prepare the sounds
// The player chopping sound
SoundBuffer chopBuffer;
chopBuffer.loadFromFile("sound/chop.wav");
Sound chop;
chop.setBuffer(chopBuffer);
// The player has met his end under a branch
SoundBuffer deathBuffer;
deathBuffer.loadFromFile("sound/death.wav");
Sound death;
death.setBuffer(deathBuffer);
// Out of time
SoundBuffer ootBuffer;
ootBuffer.loadFromFile("sound/out_of_time.wav");
Sound outOfTime;
outOfTime.setBuffer(ootBuffer);

  while (window.isOpen())
  {
    Event event;
    while (window.pollEvent(event))
    {
      if (event.type == event.Closed)
      {
        window.close();
      }
      if (event.type == Event::KeyReleased && !pause)
	{
	// Listen for key presses again
	acceptInput = true;
	// hide the axe
	spriteAxe.setPosition(2000,spriteAxe.getPosition().y);
	}
      if (Keyboard::isKeyPressed(Keyboard::Escape))
      {
        window.close();
      }
      if (Keyboard::isKeyPressed(Keyboard::Return))
      {
        pause = false;
        score = 0;
        timeRemaining = 6;
        //Dissapearing the branches
        for(int i=1;i<NUM_BRANCHES;i++){
		branchPositions[i]=side::NONE;        
        }
        // Make sure the gravestone is hidden
	spriteRIP.setPosition(675, 2000);
	// Move the player into position
	spritePlayer.setPosition(580, 720);
	acceptInput = true;
      }
      if(acceptInput){
      	if(Keyboard::isKeyPressed(Keyboard::Right)){
      		playerSide=side::RIGHT;
      		score++;
      		
      		timeRemaining+=(2/score)+.15;
      		spriteAxe.setPosition(AXE_POSITION_RIGHT,spriteAxe.getPosition().y);
      		spritePlayer.setPosition(1200,720);
      		updateBranches(score);
      		spriteLog.setPosition(810, 720);
		logSpeedX = -5000;
		logActive = true;
		acceptInput = false;
		chop.play();
      	}
      	if(Keyboard::isKeyPressed(Keyboard::Left)){
      		playerSide=side::LEFT;
      		score++;		
      		
      		timeRemaining+=(2/score)+.15;
      		spriteAxe.setPosition(AXE_POSITION_LEFT,spriteAxe.getPosition().y);
      		spritePlayer.setPosition(580,720);
      		updateBranches(score);
      		spriteLog.setPosition(810, 720);
		logSpeedX = 5000;
		logActive = true;
		acceptInput = false;
      	}
      	
      }
      
      if (Keyboard::isKeyPressed(Keyboard::P))
      {
        pause = true;
      }
    }

    Time dt = clock.restart();
    if (!pause)
    {
      timeRemaining -= dt.asSeconds();
      timeBar.setSize(Vector2f(barWidthPerSecond * timeRemaining, barHeight));

      if (timeRemaining <= 0.0f)
      {
        pause = true;
        messageText.setString("Out of time!");
        textRect = messageText.getGlobalBounds();
        messageText.setPosition(1920 / 2.0f - (textRect.width / 2), 1080 / 2.0f - (textRect.height / 2));
        outOfTime.play();
      }
      // Moving the Bee
      if (!beeActive)
      {
        srand((int)time(0));
        beeSpeed = (rand() % 200) + 200;
        srand((int)time(0) * 10);
        float height = (rand() % 500) + 500;
        spriteBee.setPosition(2000, height);
        beeActive = true;
      }
      else
      {
        spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
        if (spriteBee.getPosition().x < -100)
        {
          beeActive = false;
        }
      }
      // Moving the cloud
      if (!cloud1Active)
      {
        srand((int)time(0) * 10);
        cloud1Speed = (rand() % 200);
        srand((int)time(0) * 10);
        float height = (rand() % 150);
        spriteCloud1.setPosition(-200, height);
        cloud1Active = true;
      }
      else
      {
        spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
        if (spriteCloud1.getPosition().x > 1920)
        {
          cloud1Active = false;
        }
      }

      if (!cloud2Active)
      {
        srand((int)time(0) * 20);
        cloud2Speed = (rand() % 200);
        srand((int)time(0) * 20);
        float height = (rand() % 300) - 150;
        spriteCloud2.setPosition(-200, height);
        cloud2Active = true;
      }
      else
      {
        spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
        if (spriteCloud2.getPosition().x > 1920)
        {
          cloud2Active = false;
        }
      }

      if (!cloud3Active)
      {
        srand((int)time(0) * 30);
        cloud3Speed = (rand() % 200);
        srand((int)time(0) * 30);
        float height = (rand() % 450) - 150;
        spriteCloud3.setPosition(-200, height);
        cloud3Active = true;
      }
      else
      {
        spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
        if (spriteCloud3.getPosition().x > 1920)
        {
          cloud3Active = false;
        }
      }
      
    }
    std::stringstream ss;
    ss << "Score = " << score;
    scoreText.setString(ss.str());

    for(int i=0;i<NUM_BRANCHES;i++){
      float height=i*150;

      if(branchPositions[i] == side::LEFT){
        branches[i].setPosition(610,height);
        branches[i].setRotation(180);
      }
      else if(branchPositions[i] == side::RIGHT){
        branches[i].setPosition(1330,height);
        branches[i].setRotation(0);
      }
      else{
        branches[i].setPosition(3000,height);
      }
    }
// Handle a flying log
if (logActive)
{
spriteLog.setPosition(
spriteLog.getPosition().x +
(logSpeedX * dt.asSeconds()),
spriteLog.getPosition().y +
(logSpeedY * dt.asSeconds()));
// Has the log reached the right hand edge?
if (spriteLog.getPosition().x < -100 ||
spriteLog.getPosition().x > 2000)
{
// Set it up ready to be a whole new log next frame
logActive = false;
spriteLog.setPosition(810, 720);
}
}

// has the player been squished by a branch?
if (branchPositions[5] == playerSide)
{
// death
pause = true;
acceptInput = false;
// Draw the gravestone
spriteRIP.setPosition(525, 760);
// hide the player
spritePlayer.setPosition(2000, 660);

// Change the text of the message
messageText.setString("MAR GAYA !!");
// Center it on the screen
FloatRect textRect = messageText.getLocalBounds();
messageText.setOrigin(textRect.left +textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
messageText.setPosition(1920 / 2.0f,1080 / 2.0f);
spriteAxe.setPosition(2000,spriteAxe.getPosition().y);
death.play();
}

    window.clear();
    window.draw(spriteBackground);
    window.draw(spriteCloud1);
    window.draw(spriteCloud2);
    window.draw(spriteCloud3);
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
      window.draw(branches[i]);
    }
    window.draw(spriteTree);
    window.draw(spritePlayer);
    window.draw(spriteAxe);
    window.draw(spriteLog);
    window.draw(spriteRIP);
    window.draw(spriteBee);
    window.draw(timeBar);
    window.draw(scoreText);
    if (pause)
    {
      window.draw(messageText);
    }

    window.display();
  }
}
 void updateBranches(int seed){
  for(int j=NUM_BRANCHES-1;j>0;j--){
    branchPositions[j]=branchPositions[j-1];
  }
  srand((int)time(0)+seed);
  int r=(rand()%5);
  switch (r)
  {
  case 0:
    branchPositions[0]=side::LEFT;
    break;
  case 1:
    branchPositions[0]=side::RIGHT;
    break;
  default:
  branchPositions[0]=side::NONE;
    break;
  }
 }



