// Group:S02-Molnir
// Section 2
// link Youtube:https://youtu.be/zmHXXFmtuQs
#include <iostream>
#include <string>
#include <graphics.h>
#include <ctime>
#include <cstdlib>
#include <random>
#include <cstring>
using namespace std;

extern int bgi_textheight(const char *s) asm("_Z10textheightPKc");
#define textheight bgi_textheight


const int SCREENWIDTH = getmaxwidth();
const int SCREENHEIGHT = getmaxheight();
const int GAME_DURATION = 30;

const int GRIDSIZE = 3;
const int HOLEWIDTH = 150;
const int HOLEHEIGHT = 40;
const int HOLEGAP = 120;
const int PADDING = 70;

const int SCOREBOARD_WIDTH = 300;
const int SCOREBOARD_HEIGHT = 200;
const int MAX_MARKS = 7;
bool isEmoticonClicked = false;

bool waitForEmoticonClick(int x, int y, int size, int ms)
{
    const int step = 20; // check for a click every 20 ms
    for (int elapsed = 0; elapsed < ms; elapsed += step)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int clickX = mousex();
            int clickY = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            int dx = clickX - x; // distance from the emoticon's centre
            int dy = clickY - y;
            if (dx * dx + dy * dy <= size * size) // inside the face
                return true;
        }
        delay(step);
    }
    return false;
}

void drawHappyEmoticon(int x, int y, int size)
{
    setcolor(YELLOW); // Yellow face
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(x, y, size, size);

    setcolor(BLACK); // Black eyes
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x - size / 4, y - size / 4, size / 10, size / 10);
    fillellipse(x + size / 4, y - size / 4, size / 10, size / 10);

    setcolor(RED); // Red mouth
    setlinestyle(SOLID_LINE, 0, 3);
    arc(x, y + size / 8, 205, 335, size / 2 - size / 6);
    // Show the emoticon for 1 second and see if it gets clicked
    if (waitForEmoticonClick(x, y, size, 1000))
        isEmoticonClicked = true;
}

void drawAngryEmoticon(int x, int y, int size)
{
    setcolor(RED); // Red face
    setfillstyle(SOLID_FILL, RED);
    fillellipse(x, y, size, size);

    setcolor(BLACK); // Black eyes
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x - size / 4, y - size / 4, size / 10, size / 10);
    fillellipse(x + size / 4, y - size / 4, size / 10, size / 10);

    setcolor(BLACK); // Black eyebrows
    setlinestyle(SOLID_LINE, 0, 3);
    line(x - size / 4, y - size / 2, x - size / 8, y - size / 4);
    line(x + size / 4, y - size / 2, x + size / 8, y - size / 4);

    setcolor(BLACK); // Black mouth
    setlinestyle(SOLID_LINE, 0, 3);
    arc(x, y + size / 3 + size / 6, 25, 155, size / 2 - size / 6);
    // Show the emoticon for 1 second and see if it gets clicked
    if (waitForEmoticonClick(x, y, size, 1000))
        isEmoticonClicked = true;
}

void drawSadEmoticon(int x, int y, int size)
{
    setcolor(LIGHTBLUE); // Lightblue face
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    fillellipse(x, y, size, size);

    setcolor(BLACK); // Black eyes
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x - size / 4, y - size / 4, size / 10, size / 10);
    fillellipse(x + size / 4, y - size / 4, size / 10, size / 10);

    setcolor(BLACK); // Black mouth
    setlinestyle(SOLID_LINE, 0, 3);
    arc(x, y + size / 3 + size / 6, 25, 155, size / 2 - size / 6 + 10); // Frown arc of the mouth
    // Show the emoticon for 1 second and see if it gets clicked
    if (waitForEmoticonClick(x, y, size, 1000))
        isEmoticonClicked = true;
}

class Player;
class Marks
{
protected:
    int mark;
    Player *player;

public:
    Marks() : mark(0), player(nullptr) {}
    Marks(int initialMark, Player *player) : mark(initialMark), player(player) {}

    Player *getPlayer() const { return player; }
    void hitHappy() { mark -= 10; }
    void grabHappy() { mark += 30; }
    void hitSad() { mark += 15; }
    void grabSad() { mark -= 15; }
    void hitAngry() { mark += 20; }
    void grabAngry() { mark -= 20; }
    void setMark(int m) { mark = m; }
    int getMark() const { return mark; }

    ~Marks() {}
};

class Player
{
private:
    string name;

public:
    Player(string name = "") { this->name = name; }
    void setName(string name) { this->name = name; }
    string getName() { return name; }
    bool startGame() { return true; }
    ~Player() {}
};

class Menu
{
private:
    int buttonWidth, buttonHeight, button1, button2;
    char name[50];
    Player playerr;
    Marks playerMarks;

public:
    Menu(Player p) : playerr(p)
    {
        buttonWidth = 300;                                   // width of the button
        buttonHeight = 100;                                  // height of the button
        button1 = SCREENWIDTH / 2 - buttonWidth / 2;         // position of top button (Start Game)
        button2 = SCREENHEIGHT / 2 - buttonHeight / 2 - 100; // position of bottom button (Instructions)
    }

    Player getPlayer() { return playerr; }           // Return player instance
    void setMarks(int m) { playerMarks.setMark(m); } // Interact with Marks class
    int getMarks() { return playerMarks.getMark(); }
    // Front page menu
    void drawMainPage()
    {
        // Game title
        clearviewport();
        setbkcolor(BLACK);
        settextstyle(BOLD_FONT, HORIZ_DIR, 4);
        char *title = const_cast<char *>("EMO WHACKER");
        outtextxy(SCREENWIDTH / 2 - textwidth(title) / 2, SCREENHEIGHT / 4 - textheight(title) / 2, title);

        // Button 1 text
        settextstyle(BOLD_FONT, HORIZ_DIR, 3);
        rectangle(button1, button2 + 200, button1 + buttonWidth, button2 + buttonHeight + 200);
        char *startText = const_cast<char *>("Start Game");
        outtextxy(button1 + buttonWidth / 2 - textwidth(startText) / 2, button2 + buttonHeight / 2 - textheight(startText) / 2, startText);

        // Button 2 text
        rectangle(button1, button2, button1 + buttonWidth, button2 + buttonHeight);
        char *instructionsText = const_cast<char *>("Instructions");
        outtextxy(button1 + buttonWidth / 2 - textwidth(instructionsText) / 2, button2 + buttonHeight / 2 - textheight(instructionsText) / 2 + 200, instructionsText);
    }

    void showInstructions()
    {
        clearviewport();
        settextstyle(BOLD_FONT, HORIZ_DIR, 3);
        outtextxy(100, 100, const_cast<char *>("INSTRUCTIONS:"));
        outtextxy(100, 200, const_cast<char *>("Whack up as many marks as you can within 30 seconds!"));
        outtextxy(100, 220, const_cast<char *>("Top 3 marks are displayed on the scoreboard!"));
        outtextxy(100, 240, const_cast<char *>("Marks lower than 0 will cause the game to end."));
        outtextxy(100, 290, const_cast<char *>("Press the spacebar to alternate between the tools (Hammer - To hit, Claw - To grab)"));
        outtextxy(100, 340, const_cast<char *>("1. Grab happy emoticons. +30m"));
        outtextxy(100, 390, const_cast<char *>("2. Grab sad emoticons. -15m"));
        outtextxy(100, 440, const_cast<char *>("3. Grab angry emoticons. -20m"));
        outtextxy(100, 490, const_cast<char *>("4. Hit happy emoticons. -10m"));
        outtextxy(100, 540, const_cast<char *>("5. Hit sad emoticons once. +15m"));
        outtextxy(100, 590, const_cast<char *>("6. Hit angry emoticons twice. +20m"));
        outtextxy(100, 640, const_cast<char *>("Whack your negative emotions away! Press any key to return to main menu."));

        getch();        // Press any character to exit instructions
        drawMainPage(); // Back to main page
    }

    void menuRun()
    {
        int gd, gm;
        gd = DETECT;
        int gdriver = DETECT, gmode;

        initwindow(SCREENWIDTH, SCREENHEIGHT, "Game Menu"); // Title bar
        drawMainPage();

        char enterNameText[50] = "Enter your name: ";
        char startGameText[50] = "(Press Enter to start)";

        while (true)
        {
            if (ismouseclick(WM_LBUTTONDOWN))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                int x = mousex();
                int y = mousey();

                if (x >= button1 && x <= button1 + buttonWidth)
                {
                    if (y >= button2 && y <= button2 + buttonHeight)
                    {
                        clearviewport();
                        settextstyle(BOLD_FONT, HORIZ_DIR, 2);
                        outtextxy(SCREENWIDTH / 2 - textwidth(enterNameText) / 2, SCREENHEIGHT / 2 - textheight(enterNameText) / 2 - 60, enterNameText); // Position of text
                        outtextxy(SCREENWIDTH / 2 - textwidth(startGameText) / 2, SCREENHEIGHT / 2 - textheight(startGameText) / 2 - 20, startGameText); // Position of text
                        rectangle(SCREENWIDTH / 2 - 150, SCREENHEIGHT / 2 + 20, SCREENWIDTH / 2 + 150, SCREENHEIGHT / 2 + 50);
                        int i = 0;
                        while (true)
                        {
                            char ch = getch();         // Get any characters from the keyboard
                            if (ch == 13)              // Press enter key
                                break;                 // Run game
                            else if (ch == 8 && i > 0) // Press backspace key
                            {
                                i--;            // Delete 1 character
                                name[i] = '\0'; // Ensure the last character is null terminator
                                clearviewport();
                                outtextxy(SCREENWIDTH / 2 - textwidth(enterNameText) / 2, SCREENHEIGHT / 2 - textheight(enterNameText) / 2 - 60, enterNameText);
                                outtextxy(SCREENWIDTH / 2 - textwidth(startGameText) / 2, SCREENHEIGHT / 2 - textheight(startGameText) / 2 - 20, startGameText);
                                outtextxy(SCREENWIDTH / 2 - textwidth(name) / 2, SCREENHEIGHT / 2 + 25, name);
                            }
                            else if (ch >= 32 && ch <= 126) // Printable characters
                            {
                                name[i] = ch;       // Press characters on keyboard
                                name[i + 1] = '\0'; // Ensure the last character is null terminator
                                i++;
                                clearviewport();
                                outtextxy(SCREENWIDTH / 2 - textwidth(enterNameText) / 2, SCREENHEIGHT / 2 - textheight(enterNameText) / 2 - 60, enterNameText);
                                outtextxy(SCREENWIDTH / 2 - textwidth(startGameText) / 2, SCREENHEIGHT / 2 - textheight(startGameText) / 2 - 20, startGameText);
                                outtextxy(SCREENWIDTH / 2 - textwidth(name) / 2, SCREENHEIGHT / 2 + 25, name);
                            }
                            string playerName = name;    // Initialise the playerName
                            playerr.setName(playerName); // Set the name in player class to what they entered previously
                        }                                // End while

                        clearviewport();
                        if (playerr.startGame())
                        { // If player click enter after inserting name, start game
                            break;
                        }
                    }
                    else if (y >= button2 + 200 && y <= button2 + 200 + buttonHeight)
                    {                       // If player click instructions button
                        showInstructions(); // Show game instructions
                    }                       // End if-else-if
                }                           // End if 2
            }                               // End if 1
        }                                   // End while

        closegraph();
    }

    void drawScoreboard(int m)
    {
        const int TOTAL_MARKS = MAX_MARKS + 1;                    // 7 earlier players + this one
        int marks[TOTAL_MARKS] = {45, 26, 64, 25, 75, 85, 37, 0}; // Assume there is already marks from players before
        int centerX = SCREENWIDTH / 2;
        int centerY = SCREENHEIGHT / 2;

        marks[MAX_MARKS] = m; // Marks get from the demo player (index 7, now in range)

        // Draw scoreboard background
        setbkcolor(BLACK);
        bar(centerX - SCOREBOARD_WIDTH / 2, centerY - SCOREBOARD_HEIGHT / 2,
            centerX + SCOREBOARD_WIDTH / 2, centerY + SCOREBOARD_HEIGHT / 2);

        // Set the text color
        setcolor(WHITE);

        // Draw text labels
        settextstyle(BOLD_FONT, HORIZ_DIR, 80);
        outtextxy(centerX - 100, centerY - 60, const_cast<char *>("Scoreboard"));

        string top1, top2, top3;
        string yourMark;
        for (int i = 0; i < TOTAL_MARKS - 1; i++)
        {
            for (int j = 0; j < TOTAL_MARKS - 1 - i; j++)
            {
                if (marks[j] < marks[j + 1])
                {
                    int temp = marks[j];
                    marks[j] = marks[j + 1];
                    marks[j + 1] = temp;
                }
            }
        }
        top1 = "Player 1: " + to_string(marks[0]);
        top2 = "Player 2: " + to_string(marks[1]);
        top3 = "Player 3 : " + to_string(marks[2]);
        yourMark = "Your Mark: " + to_string(m);

        settextstyle(BOLD_FONT, HORIZ_DIR, 36);

        outtextxy(centerX - 180, centerY - 200, const_cast<char *>(yourMark.c_str()));
        outtextxy(centerX - 180, centerY + 150, const_cast<char *>(""));
        outtextxy(centerX - 180, centerY - 150, const_cast<char *>("Wall of Fame"));
        outtextxy(centerX - 180, centerY - 100, const_cast<char *>("Scoreboard"));
        outtextxy(centerX - 180, centerY + 150, const_cast<char *>(""));
        outtextxy(centerX - 180, centerY - 30, const_cast<char *>(top1.c_str()));
        outtextxy(centerX - 180, centerY + 40, const_cast<char *>(top2.c_str()));
        outtextxy(centerX - 180, centerY + 110, const_cast<char *>(top3.c_str()));
        outtextxy(centerX - 180, centerY + 110, const_cast<char *>("\n"));
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 36);
        outtextxy(centerX - 180, centerY + 250, const_cast<char *>("Press ESC to exit"));

        // Wait for a key press
        char key;
        do
        {
            key = getch();
        } while (key != 27);
    }
    ~Menu() {}
};

class Tool
{
private:
    int x, y, width, height, graphicSize, lineheight, linewidth, clickX, clickY;

public:
    Tool(int _x = 0, int _y = 0, int _width = 0, int _height = 0, int _graphicSize = 0, int _lineheight = 0, int _linewidth = 0)
    {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
        graphicSize = _graphicSize;
        lineheight = _lineheight;
        linewidth = _linewidth;
        clickX = x + graphicSize / 2;
        clickY = y + graphicSize / 2;
    }

    void drawHammer()
    {
        setcolor(BROWN);
        rectangle(x, y, x + graphicSize, y + graphicSize);
        // Draw hammer head
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        rectangle(x + 10, y + 10, x + graphicSize - 10, y + graphicSize / 2);
        floodfill(x + graphicSize / 2, y + 20, BROWN);
        // Draw hammer handle
        setfillstyle(SOLID_FILL, DARKGRAY);
        bar(x + graphicSize / 2 - 10, y + graphicSize / 2, x + graphicSize / 2 + 10, y + graphicSize);
    }

    void drawClaw()
    {
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, LIGHTMAGENTA);

        // Calculate initial claw height
        int initialClawHeight = height - graphicSize;

        // Draw the main body of the claw
        bar(x, initialClawHeight, x + width * 2, initialClawHeight + height);

        // Draw the upper part of the claw
        line(x, initialClawHeight + height, x + width / 2, initialClawHeight);
        line(x + width * 2, initialClawHeight + height, x + width + width / 2, initialClawHeight);

        // Draw the lower part of the claw
        line(x, initialClawHeight + height, x + width / 2, initialClawHeight + height * 2);
        line(x + width * 2, initialClawHeight + height, x + width + width / 2, initialClawHeight + height * 2);

        // Draw the fingers of the claw
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(x + width / 2, initialClawHeight - 10, x + width / 2 + 10, initialClawHeight + 10);
        bar(x + width + width / 2 - 10, initialClawHeight - 10, x + width + width / 2, initialClawHeight + 10);

        // Draw the claw tips
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(x + width / 2 - 20, initialClawHeight - 10, x + width / 2, initialClawHeight + 10);
        bar(x + width + width / 2, initialClawHeight - 10, x + width + width / 2 + 20, initialClawHeight + 10);
    }

    void setGraphicPosition(int mouseX, int mouseY)
    {
        x = mouseX - graphicSize / 2;
        y = mouseY - graphicSize / 2;
        clickX = x + graphicSize / 2;
        clickY = y + graphicSize / 2;
    }

    int getX() { return x; }
    int getY() { return y; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    int getGraphicSize() { return graphicSize; }
    int getClickX() { return clickX; }
    int getClickY() { return clickY; }
    ~Tool() {}
};

class Hole
{
private:
    int x, y, width, height, colour;

public:
    Hole(int x = 0, int y = 0, int width = 0, int height = 0, int colour = 0)
        : x(x), y(y), width(width), height(height), colour(colour) {}
    void draw()
    {
        // Draw the black shadow below the ellipse
        setcolor(BLACK);
        setfillstyle(SOLID_FILL, BLACK);
        fillellipse(x, y + height / 2 - 15, width / 2, height / 2);

        // Draw the ellipse
        setcolor(colour);
        setfillstyle(SOLID_FILL, colour);
        fillellipse(x, y, width / 2, height / 2);
    }

    int getX() const { return x; }
    int getY() const { return y; }

    ~Hole() {}
};

class Emoticon : public Marks
{ // Child class for Marks, Parent class for Happy,sad,Angry
protected:
    int x, y, size; // x represents the horizontal position, y represents the vertical position, size represents the dimensions of the emoticon

public:
    Emoticon() : Marks() {}
    virtual void draw(int x, int y, int size) { return; } // override function
    void setPosition(int xPos, int yPos, int emoticonSize)
    {
        x = xPos;
        y = yPos;
        size = emoticonSize;
    }
    virtual void hit() { return; }
    virtual void grab() { return; }
    virtual int getMarks() { return 0; }

    virtual ~Emoticon() {}
};

class Happy : public Emoticon
{
public:
    void draw(int x, int y, int size) { drawHappyEmoticon(x, y, size); } // Call out the happy emoticon
    void hit() { hitHappy(); }                                           // Call the hitHappy function
    void grab() { grabHappy(); }                                         // Call the grabHappy function
    int getMarks() { return getMark(); }
};

class Sad : public Emoticon
{
public:
    void draw(int x, int y, int size) { drawSadEmoticon(x, y, size); } // Call out the sad emoticon
    void hit() { hitSad(); }                                           // Call the hitSad function
    void grab() { grabSad(); }                                         // Call the grabSad function
    int getMarks() { return getMark(); }
};

class Angry : public Emoticon
{
public:
    void draw(int x, int y, int size) { drawAngryEmoticon(x, y, size); } // Call out the angry emoticon
    void hit() { hitAngry(); }                                           // Call the hitAngry function
    void grab() { grabAngry(); }                                         // Call the grabAngry function
    int getMarks() { return getMark(); }
};

class EmoWhacker
{
private:
    Hole holes[GRIDSIZE][GRIDSIZE];
    Tool hammer, claw;
    Emoticon *emo;
    int activeTool;
    string name;
    Marks mark;

public:
    EmoWhacker() : hammer(100, 100, 0, 0, 100, 30, 40), claw(100, 100, 100, 100, 100, 30, 40), emo(nullptr), activeTool(0)
    {
        name = "";
        int holeGridWidth = GRIDSIZE * HOLEWIDTH + (GRIDSIZE - 1) * HOLEGAP;
        int holeGridHeight = GRIDSIZE * HOLEHEIGHT + (GRIDSIZE - 1) * HOLEGAP;
        int startX = (SCREENWIDTH - holeGridWidth) / 2 + HOLEWIDTH / 2;
        int startY = (SCREENHEIGHT - holeGridHeight) / 2 + HOLEHEIGHT / 2;
        int offsetX = startX;
        int offsetY = startY;

        for (int row = 0; row < GRIDSIZE; row++)
        {
            for (int col = 0; col < GRIDSIZE; col++)
            {
                int x = offsetX + col * (HOLEWIDTH + HOLEGAP);
                int y = offsetY + row * (HOLEHEIGHT + HOLEGAP);
                int width = HOLEWIDTH;
                int height = HOLEHEIGHT;
                int color = RGB(101, 67, 33); // Set colour to brown
                holes[row][col] = Hole(x, y, width, height, color);
            }
        }
    }
    void setName(string n) { name = n; }
    int getMark() { return emo ? emo->getMark() : 0; }
    void drawBackground() { setbkcolor(RGB(152, 251, 152)); }

    void drawHoles()
    {
        for (int row = 0; row < GRIDSIZE; row++)
            for (int col = 0; col < GRIDSIZE; col++)
                holes[row][col].draw();
    }

    void drawTools()
    {
        if (activeTool == 0)
        {
            hammer.setGraphicPosition(mousex(), mousey()); // move first, then draw,
            hammer.drawHammer();                           // or the tool lags a frame
        }
        else
        {
            claw.setGraphicPosition(mousex(), mousey());
            claw.drawClaw();
        }
    }

    void onMouseMove(int mouseX, int mouseY)
    {
        if (activeTool == 0)
            hammer.setGraphicPosition(mouseX, mouseY);
        else
            claw.setGraphicPosition(mouseX, mouseY);
    }

    void switchTool()
    {
        activeTool = (activeTool + 1) % 2;
        onMouseMove(mousex(), mousey());
    }

    void displayDashboard(string name, int marks, int timeRemaining)
    {
        settextstyle(BOLD_FONT, HORIZ_DIR, 3);
        setcolor(BLACK);

        string playerName = "Player: " + name;
        char playerNameStr[playerName.length() + 1];
        strcpy(playerNameStr, playerName.c_str());

        string playerMarks = "Marks: " + to_string(marks);
        char marksStr[playerMarks.length() + 1];
        strcpy(marksStr, playerMarks.c_str());

        string timeStr = "Countdown: " + to_string(timeRemaining) + " s";
        char timeRemainingStr[timeStr.length() + 1];
        strcpy(timeRemainingStr, timeStr.c_str());

        int leftX = 10;                                            // Left position
        int centerX = getmaxx() / 2 - textwidth(marksStr) / 2;     // Center position
        int rightX = getmaxx() - textwidth(timeRemainingStr) - 10; // Right position

        outtextxy(leftX, 10, playerNameStr);
        outtextxy(centerX, 10, marksStr);
        outtextxy(rightX, 10, timeRemainingStr);
    }

    void run()
    {
        initwindow(SCREENWIDTH, SCREENHEIGHT, "Emo Whacker");
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        int startTime = static_cast<int>(time(nullptr));
        int currentTime = startTime;
        int playerMarks = 0;

        while (currentTime - startTime < GAME_DURATION)
        {
            cleardevice();
            drawBackground();
            drawHoles();
            drawTools();
            displayDashboard(name, playerMarks, GAME_DURATION - (currentTime - startTime));

            if (GetAsyncKeyState(VK_SPACE))
                switchTool();
            // Generate random positions for emoticons
            int randomRow = rand() % GRIDSIZE;
            int randomCol = rand() % GRIDSIZE;
            int x = holes[randomRow][randomCol].getX(); // Get x-coordinate of the hole
            int y = holes[randomRow][randomCol].getY(); // Get y-coordinate of the hole

            if (playerMarks >= 0)
            {
                // Emoticons size
                int size = 60;

                // Generate random emoticon type
                int emoticonType = rand() % 3; // 0 for happy, 1 for sad, 2 for angry

                // Draw the corresponding emoticon based on the random type
                delete emo; // free last frame's emoticon before allocating another
                emo = nullptr;
                if (emoticonType == 0)
                {
                    emo = new Happy();
                }
                else if (emoticonType == 1)
                {
                    emo = new Sad();
                }
                else if (emoticonType == 2)
                {
                    emo = new Angry();
                }
                emo->draw(x, y, size);
                // add score
                if (isEmoticonClicked)
                {
                    if (activeTool == 0)
                    {
                        emo->hit();
                    }
                    else
                    {
                        emo->grab();
                    }
                    playerMarks += emo->getMarks();
                }
            }
            else
            {
                if (emo) emo->setMark(playerMarks);
                break;
            }
            currentTime = static_cast<int>(time(nullptr));
            isEmoticonClicked = false;
        }
        if (emo) emo->setMark(playerMarks);
    }
    ~EmoWhacker() { delete emo; }
};

int main()
{
    Player player("");                                // Default player name
    Menu menu(player);                                // Pass default player name to menu
    menu.menuRun();                                   // Display menu
    string namePlayer = (menu.getPlayer()).getName(); // Get the player name entered
    EmoWhacker game;
    game.setName(namePlayer); // Pass the player name entered to be displayed during the game
    game.run();
    menu.drawScoreboard(game.getMark());
    return 0;
}