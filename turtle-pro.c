#include "imgUtils.c"
#include <string.h>

// This lets the driver code override the image size if it needs to. Make sure
// you don't hard-code these values anywhere!
#ifndef SIZEX
#define SIZEX 512
#define SIZEY 512
#endif

/*---------------------------------------------------------------------------*/

/**
 * This struct contains one node of the linked list, which represents a single 
 * command to the Turtle. It's field should include:
 * 
 *  - cmd   : A char array of size 10 holding the command name
 * 
 *  - val   : An integer that stores a parameter for the command (like forward,
 *            backward and colour). 
 * 
 *  - next  : A pointer to a struct of the same type, this is used for the 
 *            linked list
 * 
 * TODO: Complete this struct definition
 ****/

#define SIZE 10

typedef struct cmdnode
{
    char cmd[SIZE];
    int val;
    struct cmdnode *loop_head;
    struct cmdnode *next;
} CmdNode;

/*---------------------------------------------------------------------------*/
typedef struct cmd
{
    char command[SIZE];
} Cmd;

int countCommands(CmdNode *head);

int is_valid_cmd(char *cmd)
{

    const int num_of_cmds = 8;
    Cmd cmds[] = {{"penup"}, {"pendown"}, {"right"}, {"left"}, {"forward"}, {"backward"}, {"colour"}, {"loop"}};
    int found = 0;
    int i = 0;

    while (i < num_of_cmds && found != 1)
    {
        if (strcmpi(cmds[i].command, cmd) == 0)
            found = 1;
        i++;
    }

    if (found == 0)
        printf("Invalid command.\n");

    return found;
}

CmdNode *newCmdNode(char cmd[10], int val)
{
    /** 
   * This function allocates a new CmdNode struct and initializes it's values 
   * based on the input paramaters given. The next pointer is always 
   * initialized to NULL.
   * 
   * If the 'cmd' parameter is not a correct command, then print
   * "Invalid command.\n" and return NULL.
   * 
   * Note that we will always pass in a value here, even if the 
   * command doesn't need one. In this case, we can just ignore 
   * it. It saves us from having to make separate functions to 
   * deal with this.
   * 
   * TODO: Implement this function
   */

    if (is_valid_cmd(cmd))
    {
        CmdNode *new_node = (CmdNode *)calloc(1, sizeof(CmdNode));
        if (new_node == NULL)
            return NULL;
        strcpy(new_node->cmd, cmd);
        new_node->val = val;
        new_node->next = NULL;
        new_node->loop_head = NULL;
        return new_node;
    }

    return NULL;
}

/*---------------------------------------------------------------------------*/
int val_cmds(char *cmd)
{
    Cmd cmds[] = {{"forward"}, {"backward"}, {"colour"}, {"loop"}};
    for (int i = 0; i < 4; i++)
    {
        if (strcmpi(cmds[i].command, cmd) == 0)
            return 1;
    }
    return 0;
}

void printCommandList(CmdNode *head)
{
    /**
   * This function prints out each command in the linked list one after the 
   * other. Each command MUST also have a line number printed before it, this 
   * is what you will be using to modify / delete them. To do this, initialize 
   * a counter and then increment it for each command.
   * 
   * Depending on whether or not the command needs an additional value 
   * (like forward, backward and colour), use one of the following statements 
   * to print out the information for each node: 
   *            [ The format is "linenumber: command value" ]
   * 
   * printf("%3d: %s %d\n", ... );     [With a value]
   * 
   * printf("%3d: %s\n", ... );        [Without a value]
   * 
   * Obviously, you also need to pass in the correct parameters to the print 
   * function yourself, this is just so you have the correct format.
   * 
   * TODO: Implement this function
   */

    int list_len = countCommands(head);
    int i = 0;

    for (i = 0; i < list_len; i++)
    {
        if (val_cmds(head->cmd))
        {
            printf("%3d: %s %d\n", i, head->cmd, head->val);

            if (!strcmpi(head->cmd, "loop")) // prints the loops cmds if any
            {
                CmdNode *loop = NULL;
                loop = head->loop_head;
                while (loop != NULL)
                {
                    i++;
                    if (val_cmds(loop->cmd))
                        printf("%3d:   %s %d\n", i, loop->cmd, loop->val);

                    else
                        printf("%3d:   %s\n", i, loop->cmd);

                    loop = loop->next;
                }
            }
        }
        else
            printf("%3d: %s\n", i, head->cmd);

        head = head->next;
    }
    return;
}

/*---------------------------------------------------------------------------*/

void queryByCommand(CmdNode *head, char cmd[10])
{
    /**
   * This function looks for commands in the linked list that match the input 
   * query. It prints them out in the same format as the printCommandList() 
   * function.
   * 
   * Make sure that the line number of the commands that match is the same as 
   * the line number that would be printed by the printCommandList() function.
   * 
   * --------------------------------------------------------------------------
   * 
   * For instance, if your printCommandList function outputs
   * 
   *    0: penup
   *    1: forward 200
   *    2: right
   *    3: forward 50
   * 
   * Then, if this function is called with the same list and cmd = "forward", 
   * then your output here should be
   * 
   *    1: forward 200
   *    3: forward 50
   * 
   * TODO: Implement this function
   */

    if (!is_valid_cmd(cmd)) //exits if not valid command
        return;

    int list_len = countCommands(head);
    CmdNode *temp = NULL;

    for (int i = 0; i < list_len; i++)
    {
        if (!strcmpi(head->cmd, cmd))
        {
            if (val_cmds(head->cmd))
                printf("%3d: %s %d\n", i, head->cmd, head->val);
            else
                printf("%3d: %s\n", i, head->cmd);
        }

        if (head->loop_head != NULL)
        {
            // then it is a loop node
            temp = head;
            head = head->loop_head;
        }
        else if (temp != NULL && head->next == NULL)
        {
            head = temp->next;
            temp = NULL;
        }
        else
            head = head->next;
    }

    return;
}

/*---------------------------------------------------------------------------*/

int countCommands(CmdNode *head)
{
    /**
   * This function counts and returns the length of the linked list. It 
   * requires list traversal.
   * 
   * TODO: Implement this function
   */
    int counter = 0;

    while (head != NULL)
    {
        counter++; // counts the node that says loop, cause thats a valid command too
                   // then proceeds with the looped commands if there is one
        if (strcmpi(head->cmd, "loop") == 0)
        {
            CmdNode *loop = NULL;
            loop = head->loop_head;
            while (loop != NULL)
            {
                counter++;
                loop = loop->next;
            }
        }
        head = head->next;
    }
    return counter;
}

/*---------------------------------------------------------------------------*/

CmdNode *insertCommand(CmdNode *head, CmdNode *new_CmdNode)
{
    /**
   * This function inserts the node new_CmdNode *at the tail* of the linked 
   * list. (You are adding a command at the end).
   * 
   * If head == NULL, then the linked list is still empty.
   * 
   * It returns a pointer to the head of the linked list with the new node 
   * added into it.
   * 
   * TODO: Implement this function
   */
    if (head == NULL)
        return new_CmdNode;

    CmdNode *ptr = NULL;
    CmdNode *head_ptr = NULL;
    head_ptr = head;

    while (head != NULL)
    {
        ptr = head;
        head = head->next;
    }
    ptr->next = new_CmdNode;
    return head_ptr;
}

/*---------------------------------------------------------------------------*/

CmdNode *get_cmdnum_node(CmdNode *head, int cmdNum)
{

    // takes the head node of the linked list and
    // the node num of which we want to find (cmdNum)
    // returns the pointer of found node

    if (cmdNum >= countCommands(head) || cmdNum < 0)
    {
        printf("Invalid Command Number.\n");
        return NULL;
    }

    CmdNode *node_ptr = NULL;
    for (int i = 0; i <= cmdNum; i++)
    {
        node_ptr = head;
        head = head->next;
    }
    return node_ptr;
}

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

CmdNode *deleteCommandList(CmdNode *head)
{
    /**
   * This function deletes the linked list of commands, releasing all the 
   * memory allocated to the nodes in the linked list.
   * 
   * Returns a NULL pointer so that the head of the list can be set to NULL
   * after deletion.
   * 
   * TODO: Implement this function
   */

    // this code was taken from unit 3
    CmdNode *p = NULL;
    CmdNode *q = NULL;
    p = head;
    while (p != NULL)
    {
        if (strcmpi(p->cmd, "loop") == 0)
        {
            CmdNode *a = p->loop_head;
            CmdNode *b = NULL;
            while (a != NULL)
            {
                b = a->next;
                free(a);
                a = b;
            }
        }

        q = p->next;
        free(p);
        p = q;
    }

    return NULL;
}

/*---------------------------------------------------------------------------*/

void insertCommandLoop(CmdNode *loop_node, CmdNode *new_node)
{
    /**
     This function will be used to insert
     a command at the end of the linked list for a 'loop' node. It will take in 2
     parameters, the first will be a pointer to the loop node, and the second will 
     be a pointer to the new command node that you have to insert into the loop. 
     This function should not return anything, but just update the linked list at 
     the loop node.
     *
     **/

    CmdNode *trail = NULL;
    trail = loop_node->loop_head;
    if (loop_node == NULL)
        return;
    if (trail == NULL) // if loop LL is empty then link loop_head with new node
    {
        loop_node->loop_head = new_node;
        return;
    }

    loop_node = loop_node->loop_head; // into the loop commands

    while (loop_node->next != NULL) // traversal to tail 
    {
        loop_node = loop_node->next;
    }

    loop_node->next = new_node;
    return;
}

/*---------------------------------------------------------------------------*/
#define TRUE 1
#define FALSE 0

typedef enum direction
{
    up,
    right,
    down,
    left
} Direction; //made so its easy to know with dir in ints rather than comparing strings

int is_pendown(char *cmd)
{
    // if output is 1 then pendown
    // if output is 0 then penup
    // if neither then -1
    if (strcmpi(cmd, "pendown") == 0)
    {
        return 1;
    }
    if (strcmpi(cmd, "penup") == 0)
    {
        return 0;
    }
    return -1;
}

void check_pos_y(int *pos_y)
{
    if (*pos_y > SIZEY - 1)
    {
        *pos_y = SIZEY - 1;
    }
    else if (*pos_y < 0)
    {
        *pos_y = 0;
    }
}

void check_pos_x(int *pos_x)
{
    if (*pos_x > SIZEX - 1)
    {
        *pos_x = SIZEX - 1;
    }
    else if (*pos_x < 0)
    {
        *pos_x = 0;
    }
}

void forward(int *pos_x, int *pos_y, int val, Direction dir)
{
    if (val < 0)
    {
        return;
    } // negative value will turn it into a backwards function

    switch (dir)
    {
    case (up):
        *pos_y -= val;
        check_pos_y(pos_y);
        break;

    case (down):
        *pos_y += val;
        check_pos_y(pos_y);
        break;

    case (left):
        *pos_x -= val;
        check_pos_x(pos_x);
        break;

    case (right):
        *pos_x += val;
        check_pos_x(pos_x);
        break;

    default:
        break;
    }
}

void backward(int *pos_x, int *pos_y, int val, Direction dir)
{
    if (val < 0)
    {
        return;
    } // negative value will turn it into a forward function

    switch (dir)
    {
    case (up):
        *pos_y += val;
        check_pos_y(pos_y);
        break;

    case (down):
        *pos_y -= val;
        check_pos_y(pos_y);
        break;

    case (left):
        *pos_x += val;
        check_pos_x(pos_x);
        break;

    case (right):
        *pos_x -= val;
        check_pos_x(pos_x);
        break;

    default:
        break;
    }
}

void turn(char *cmd, Direction *dir)
{
    // turns left or right
    // code starts with pointing right
    if (strcmpi(cmd, "right") == 0 && *dir != left)
    {
        *dir += 1;
    }
    else if (strcmpi(cmd, "right") == 0 && *dir == left)
    {
        *dir = up;
    }

    if (strcmpi(cmd, "left") == 0 && *dir != up)
    {
        *dir -= 1;
    }
    else if (strcmpi(cmd, "left") == 0 && *dir == up)
    {
        *dir = left;
    }
}

void perform_cmd(CmdNode *head, int *pos_x, int *pos_y, Direction *dir, int *colour,
                 Image *im, int *start_x, int *start_y);

void do_loop(CmdNode *head, int *pos_x, int *pos_y, Direction *dir, int *colour,
             Image *im, int *start_x, int *start_y)
{

    if (head->loop_head == NULL)
    {
        return;
    }
    int loop_times = head->val; // num of times to loop
    head = head->loop_head;
    CmdNode *orginal_head = head;

    for (int i = 0; i < loop_times; i++)
    {
        while (head != NULL)
        {
            *start_x = *pos_x;
            *start_y = *pos_y;
            perform_cmd(head, pos_x, pos_y, dir, colour, im, start_x, start_y);
            drawLine(im, *start_x, *start_y, *pos_x, *pos_y, *colour);
            head = head->next;
        }
        head = orginal_head;
    }
}

void perform_cmd(CmdNode *head, int *pos_x, int *pos_y, Direction *dir,
                 int *colour, Image *im, int *start_x, int *start_y)
{

    if (strcmpi(head->cmd, "forward") == 0)
    {
        forward(pos_x, pos_y, head->val, *dir);
    }
    else if (strcmpi(head->cmd, "backward") == 0)
    {
        backward(pos_x, pos_y, head->val, *dir);
    }
    else if (strcmpi(head->cmd, "left") == 0 || strcmpi(head->cmd, "right") == 0)
    {
        turn(head->cmd, dir);
    }
    else if (strcmpi(head->cmd, "colour") == 0)
    {
        if (head->val > 255)
        {
            *colour = 255;
        }
        else if (head->val < 0)
        {
            *colour = 0;
        }
        else
        {
            *colour = head->val;
        }
    }
    else if (strcmpi(head->cmd, "loop") == 0)
    {
        do_loop(head, pos_x, pos_y, dir, colour, im, start_x, start_y);
    }
}

void run(Image *im, CmdNode *head, int *endX, int *endY)
{
    /**
   * This function runs the list of commands to move the turtle around and draw 
   * the image, and returns the final position of the turtle in the variables 
   * endX and endY.
   * 
   * --------------------------------------------------------------------------
   * 
   * NOTE: In the image we work with, the top-left pixel is (0,0),
   *       - x increases as you go right, up till SIZEX-1
   *       - y increases as you go down, up till SIZEY-1
   * 
   *                 (0,0)                 (SIZEX-1, 0)
   *                   x------------------------x
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |          IMAGE         |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   x------------------------x
   *             (0, SIZEY-1)            (SIZEX-1, SIZEY-1)
   * 
   * The image is in grayscale (black and white), so the colours are numbers 
   * from [0-255] where 0 is black, 255 is white, and the values in between 
   * are varying levels of gray.
   * 
   * You need to understand how the (x,y) values are stored so you know how 
   * they should be updated when you move (down means y increases, etc). You do 
   * not need to use the 'im' variable for anything other than passing it into 
   * the drawLine() function described below.
   * 
   * --------------------------------------------------------------------------
   * 
   * Here's the setup - There is a turtle (with a pen) that walks along the 
   * image. When the pen is down (on the paper), it draws a line along the path 
   * that it walks on. (If you want to play around with this, consider looking 
   * at the `turtle` library in python!)
   * 
   * The turtle initially starts at pixel (0, 0) [at the top left], 
   * facing right (in the positive x direction). The pen starts off
   * as `down`, with the default colour being black (0).
   * 
   * You need to go through the linked list and 'run' the commands to keep 
   * track of the turtles position, and draw the appropriate lines. Here is 
   * what each command should do:
   * 
   *  - penup          : Put the pen up (stop drawing)
   *  - pendown        : Put the pen down (start / continue drawing)
   *  - colour <val>   : Draw lines in colour <val> from now on
   *  - forward <val>  : Move the turtle forward <val> steps (pixels) 
   *                     in the direction it is facing.
   *  - backward <val> : Same as above, except move backwards
   *  - right          : Turn the turtle to the right by 90 degrees
   *  - left           : Turn the turtle to the left by 90 degrees
   * 
   * NOTE: Make sure that you do *not* go outside the image. For this, set the 
   * maximum and minimum values of x and y to be 0 and SIZEX-1 / SIZEY-1 
   * respectively.
   * 
   * For instance, if the turtle is at (0,0) facing right, and your command is
   * `forward 100000`, it will only go forward till (SIZEX-1, 0), and end 
   * up at the rightmost pixel in that row.
   * 
   * IMPORTANT: Once you are done with all the commands, make sure you save the 
   * final (x,y) location of the turtle into the variables endX and endY.
   * 
   * --------------------------------------------------------------------------
   * 
   * We have already implemented a drawLine() function (in imgUtils.c) which 
   * you should use to actually draw the lines. It takes in the following:
   * 
   *      - a pointer to an image struct (use 'im')
   *      - (x,y) location of start point
   *      - (x,y) location of end point
   *      - a colour to draw the line in [0-255]
   *          
   * Note that this function only draws horizontal and vertical lines, so 
   * either the x values or the y values of both points must be the same. 
   * Both these points *must* also be within the image. 
   *          [ 0 <= x < SIZEX,  0 <= y < SIZEY ]
   * 
   * 
   * TODO: Implement this function
   */

    int pen_cmd = 1;    // tells us which cmd, if -1 then neither penup or pendown
                        // 0 is penup, 1 is pendown
    int pendown = TRUE; // 0 is pendown false(penup), 1 is pendown true

    Direction dir = right;
    int colour = 0;
    int start_x = 0, start_y = 0;
    *endY = 0, *endX = 0;

    while (head != NULL)
    {
        {
            pen_cmd = is_pendown(head->cmd);
            if (pen_cmd == FALSE || pen_cmd == TRUE)
            {
                pendown = pen_cmd;
            }

            switch (pendown)
            {
            case (FALSE):
                // if pendown false then
                // we dont draw
                start_x = *endX; //old ending pos becomes new starting pos
                start_y = *endY;
                perform_cmd(head, endX, endY, &dir, &colour, im, &start_x, &start_y);
                break;

            case (TRUE):
                // if pendown true
                // we draw but only
                // in straight line
                start_x = *endX; //old ending pos becomes new starting pos
                start_y = *endY;
                perform_cmd(head, endX, endY, &dir, &colour, im, &start_x, &start_y);
                if (strcmpi(head->cmd, "loop") != 0)
                {
                    drawLine(im, start_x, start_y, *endX, *endY, colour);
                }      //since we are already drawing in another
                break; //function we skip it if cmd is loop cmd
            }

            head = head->next;
        }
    }

    return;
}

/*---------------------------------------------------------------------------*/
// All done!