Command-based system:
* Split into input handling and game logic
Events are polled in Game and forwarded to Player ->
transforms events to commands and feeds CommandQueue with them.
* Same process for real-time input
Player checks current input state and pushes commands to the queue.
* CommandQueue stores a queue of commands and acts as a bridge between input
handling and game logic.
* Game logic side:
World pops commands from CommandQueue and sends them to the root of the
scene graph ->
then distributed depending on their reciever categories

Input handling -> encapsulated in Player obj
* Make components generic and reusable
Command and CommandQueue can be used generically for other func too
    ex. network or AI
