# Behavioral

Behavioral design patterns in C++17: object collaboration and responsibility (Chain of Responsibility, Command, Iterator, Mediator, Memento, Observer, State, Strategy, Template Method, Visitor), one self-contained demo per pattern.

## Files
- `ChainOfResponsibility.cpp` - handlers each handle a request or pass it to the next link
- `Command.cpp` - light on/off requests as objects for a remote control
- `Iterator.cpp` - playlist traversal with a nested Iterator class (begin/end, operator!=)
- `Mediator.cpp` - colleagues communicate only through a mediator
- `Memento.cpp` - originator snapshots, opaque Memento, caretaker history
- `Observer.cpp` - weather station notifying attached observers (raw `Observer*` list)
- `State.cpp` - media player behaviour switches with its playback state object
- `Strategy.cpp` - interchangeable payment algorithms selected at runtime
- `TemplateMethod.cpp` - fixed document-export skeleton with per-format steps
- `Visitor.cpp` - visit overload per element type (double dispatch) over shapes

## Build and run
- Single file: `g++ -std=c++17 -Wall -Wextra -pthread ChainOfResponsibility.cpp -o /tmp/x && /tmp/x` (swap in any other file listed above).
- `make` from `Cpp/code` builds each file into `bin/DesignPatterns/<Group>/<Name>` (git-ignored).

## Key concepts / interview angles
- Strategy vs State: both swap an object behind an interface; Strategy is chosen by the client, State transitions itself.
- Observer: lifetime of observers is the classic bug (dangling `Observer*`); use weak_ptr or explicit detach.
- Command: enables undo/redo, queues and macros.
- Visitor: adds operations without editing element classes but makes adding new element types hard; compare `std::visit`.
- Template Method: inversion of control via base-class skeleton; compare Strategy (composition).
- Memento: snapshot without breaking encapsulation.

## Related
- `../../20_ModernCppFeatures/07_stdVisitVariant.cpp`
- `../../16_Lambdas`
- `../../10_Polymorphism`
- `../../31_SOLIDPrinciples`

## Conventions when extending
- New files use a two-digit prefix plus descriptive name (e.g. `05_foo.cpp`); keep each `.cpp` self-contained and independently compilable.
- Keep NOTES.md concise, bullet-style and interview-focused; never commit binaries (`Cpp/code/bin/` is git-ignored).
