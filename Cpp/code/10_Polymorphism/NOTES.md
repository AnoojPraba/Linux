# Polymorphism -- interview notes

## Virtual destructor necessity (`02_virtualDestructor.cpp`)

- Deleting a derived object through a base class pointer when the base
  destructor is NOT virtual is undefined behavior. In practice, the
  observable symptom is that the derived part's destructor (and therefore its
  cleanup) does not run.
- The fix is simply marking the base destructor `virtual`, which makes
  `delete basePointer;` correctly run the most-derived destructor first, then
  chain up through each base class's destructor.
- Rule of thumb / classic interview question: any class that has at least one
  virtual function, or is otherwise intended as a polymorphic base at all,
  should have a virtual destructor -- even if that destructor's body is empty.
  If a class is not intended to be used polymorphically (no virtual functions,
  never deleted through a base pointer), a virtual destructor isn't required,
  but it costs little and it's easy to forget the rule when a class later
  grows into a polymorphic base.
