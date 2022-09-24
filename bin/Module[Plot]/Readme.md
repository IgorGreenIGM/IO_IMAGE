/*
 * @author : Mogou igor green
 * @mail : igormogou86@gmail.com
 * @file : Readme.md
 * @date : 2022-7-15 10:31
*/

This is a graph ploter program written in C and fully compatible with C++ (tested with gcc and g++)
Which can draw a graph from datas stored in a single or multiple input files dynamically.


Usage : 
    
    to plot a file, you just need to specify the file path *after* the '-istream' command line option when starting the program
    for example : ploter.exe-istream "filePath.txt"
    then to plot many files, just some -istream commands followed by the file path
    example : ploter.exe -istream path1.txt -istream path2.txt ...


Options :

    The ploter has many options that affect the graph displaying : 
         "-istream" : specify an input file path
         "-grid" : draw a grid to the graph background, this is selected by default
         "-full" : open the window graph in full screen mode (exit with Escape key)
         "-axis" : draw a vertical and a horizontal axis to the graph background, this is selected by default
         "-info" : display informations (in th shell) of the differents specified files (max value, min value, size, scale, ...)
         "-animation" : display the differents graph as an animation 

         "-no-axis" : disable axis displaying option
         "-no-grid" : disable grid displaying option


Running :

    when you finished specifying the differents files and options and start the program, these are occuring :
        - the program start for each command (default commands such as "-grid" and "-axis" and users commands) initialize a struct that facilitate the command treatment (separing the first character, the command string and a pointer to the whole command)
        - Then it creates a linked list and add in all the specified initialized struct.  
        - It analyse all the List and treat all the non specified commands (not appearing it the described Options list), and for recognized commands, it enable or disable each specific.
        - Then if there is one or mulptiples valid files in the commands, the program search the max value of each file generate a scale then all the graph should be corrected ploted at the screen.
        - After generating scale, the program read all the points in the streams and stores it in buffers, then from these buffers and generated scales we start drawing each graph with a randomly generated color, either animated or not, according to the commands options.
        note :: default commands treatment occurs before users commands.


Notes :

    [Librairies] : OpenGl for drawing methods | GLUT API for window and loading opengl context.
    [Examples] : a executable windows file is provided with 03 graph datas.
    [Important] : this ploter only support positives or nuls values.
                  in an input file, only one point should appear on each line, x value separed to y by a space ' '.
                  graphs are ploted in the order of the transmission of commands

Features:

    - manage negatives datas
    - add a legend so we could see which file is ploted with with color
    - add a mouse with interaction
    - add a for navigating and zoom
    - add a file manager which can add any other file even files are already ploted