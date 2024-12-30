// Made by ThatOneGeekyGuy
// Github: https://github.com/ThatOneGuy511/FlipperZero-InputDemo-App


#include <input/input.h>    // Handles input
#include <gui/gui.h>        // Handles GUI
#include <furi.h>           // Handles the furi logging

#define TAG "Input Demo"   // Tag for furi logging

typedef struct {
    FuriMessageQueue* input_queue;  // Pointer to store user inputs
    ViewPort* view_port;            // Pointer to handle display
    Gui* gui;                       // Pointer to current GUI
    char pressedKey[32];            // Place holder for holding the current key pressed
} inputDemo;

void draw_callback(Canvas* canvas, void* context) {
    //inputDemo app;
    UNUSED(context);

    inputDemo* app = (inputDemo*)context;


    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);

    canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, app->pressedKey);

}

void input_callback(InputEvent* event, void* context) {
    inputDemo* app = context;
    furi_message_queue_put(app->input_queue, event, 0);
}

int32_t input_demo_main(void*) {

    // Store all the data structure in variable 'app'
    inputDemo app;

    // Allocate view_port and furi logging
    app.view_port = view_port_alloc();
    app.input_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    // Set the callbacks function to draw on the GUI and inputs
    view_port_draw_callback_set(app.view_port, draw_callback, &app);
    view_port_input_callback_set(app.view_port, input_callback, &app);

    // Add a GUI to the allocated ViewPort
    app.gui = furi_record_open("gui");
    gui_add_view_port(app.gui, app.view_port, GuiLayerFullscreen);

    // Set greeting message 
    strcpy(app.pressedKey, "Press a Key");
    view_port_update(app.view_port);

    // Input handling loop
    InputEvent input;
    bool exit_loop = false;
    FURI_LOG_I(TAG, "Start the main loop.");
    while(1) {

        // Check if there is an error in the 'furi_check' function
        furi_check(furi_message_queue_get(app.input_queue, &input, FuriWaitForever) == FuriStatusOk);


        // Assign that input to a function
        switch(input.key) {
        case InputKeyLeft:
            strcpy(app.pressedKey, "Left");
            break;
        case InputKeyRight:
            strcpy(app.pressedKey, "Right");
            break;
        case InputKeyOk:
            strcpy(app.pressedKey, "Ok/Enter");
            break;
        case InputKeyUp:
            strcpy(app.pressedKey, "Up");
            break;
        case InputKeyDown:
            strcpy(app.pressedKey, "Down");
            break;
        case InputKeyBack:
            strcpy(app.pressedKey, "Back/Return");
            
            // Hold down for 1 seconds to exit
            if (input.type == InputTypeLong)
                exit_loop = true;
            
            break;

        default:
            break;
        }

        if (exit_loop == true) {
            break;
        }

        // Update the viewport every itineration of the loop
        view_port_update(app.view_port);
    }

    // Free structures and exit
    view_port_enabled_set(app.view_port, false);    // Disable ViewPort
    gui_remove_view_port(app.gui, app.view_port);   // Remove the GUI from the ViewPort
    furi_record_close("gui");                       // Close the furi record fo the GUI
    view_port_free(app.view_port);                  // Then free the ViewPort

    return 0;   
}
