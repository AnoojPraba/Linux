# Inheritance -- interview notes

## Object slicing (`02_objectSlicing.cpp`)

- Assigning or passing a Derived object BY VALUE into a Base-typed
  variable/parameter "slices" it: only the Base subobject is copied, because
  the copy goes through Base's own copy constructor, which has no knowledge
  of any Derived-only members.
- This is not a compile error -- it silently compiles and quietly drops data,
  which makes it a classic source of hard-to-spot bugs.
- The fix/takeaway: pass or store polymorphic types by reference or pointer
  (`Base &`, `Base *`, or a smart pointer), never by value, whenever slicing
  would matter. References/pointers don't copy the object at all, so nothing
  gets sliced.
