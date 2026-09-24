# SOLID Principles

- **S — Single Responsibility Principle**: a class should have only one reason to
  change. Smell: a class that mixes unrelated concerns (e.g. computing data,
  formatting/printing it, and persisting it, as in `01_singleResponsibility.cpp`'s
  `ReportBad`). Fix: split each concern into its own class (`ReportCalculator`,
  `ReportPrinter`, `ReportSaver`) so each has a single axis of change.
- **O — Open/Closed Principle**: software entities should be open for extension but
  closed for modification. Smell: a function with a big `if`/`switch` on a type tag
  that must be edited every time a new case is added (`02_openClosed.cpp`'s
  `areaBad`). Fix: introduce a polymorphic base (`Shape`) and add new behavior via
  new subclasses (`Circle`, `Rectangle`) without touching existing code.
- **L — Liskov Substitution Principle**: subtypes must be substitutable for their
  base type without breaking the base's contract/invariants. Smell: the classic
  Rectangle/Square case where overriding `setWidth`/`setHeight` on `Square` changes
  both dimensions together, breaking code that assumes independence
  (`03_liskovSubstitution.cpp`). Fix: don't force an is-a relationship where
  behavior isn't substitutable — model the shared capability (`area()`) via a
  common interface (`ShapeWithArea`) instead of inheriting one concrete shape from
  another.
- **I — Interface Segregation Principle**: clients shouldn't be forced to depend on
  methods they don't use. Smell: one fat interface (`MultiFunctionDeviceBad` with
  `print`/`scan`/`fax`) that forces implementers lacking some capability to stub out
  or throw from irrelevant methods (`04_interfaceSegregation.cpp`). Fix: split into
  small, focused interfaces (`Printable`, `Scannable`, `Faxable`) and implement only
  the ones that apply.
- **D — Dependency Inversion Principle**: high-level modules shouldn't depend on
  low-level modules; both should depend on abstractions. Smell: a high-level class
  directly `new`-ing/holding a concrete low-level type (`UserServiceBad` holding a
  `MySqlDatabase`), coupling policy to one implementation and making it untestable
  (`05_dependencyInversion.cpp`). Fix: depend on an abstraction (`Database`) and
  inject the concrete implementation via the constructor (constructor injection),
  as `UserService` does.

## Cross-references to DesignPatterns/

- **OCP**: `DesignPatterns/Behavioral/Strategy.cpp` (new payment strategies added as
  new `PaymentStrategy` subclasses, `ShoppingCart` never changes) and
  `DesignPatterns/Behavioral/Observer.cpp` (new observer types can be added without
  modifying the subject) both embody OCP directly. `DesignPatterns/Behavioral/State.cpp`
  and `DesignPatterns/Creational/FactoryMethod.cpp` / `AbstractFactory.cpp` also let new
  variants be added via new subclasses/factories rather than editing existing branches.
- **ISP-adjacent goals**: `DesignPatterns/Structural/Adapter.cpp` and
  `DesignPatterns/Structural/Facade.cpp` narrow or reshape a broad/incompatible
  interface into exactly what a client needs, which is in the same spirit as ISP
  (give clients only the surface they actually use). `DesignPatterns/Structural/Proxy.cpp`
  and `DesignPatterns/Structural/Decorator.cpp` preserve a single focused interface
  while adding behavior transparently.
- **DIP**: `DesignPatterns/Behavioral/Strategy.cpp` also supports DIP — `ShoppingCart`
  depends on the abstract `PaymentStrategy` interface, not a concrete payment class,
  with the concrete strategy injected via `setPaymentStrategy`.
  `DesignPatterns/Creational/AbstractFactory.cpp` and `FactoryMethod.cpp` let
  high-level code depend on an abstract creation interface instead of concrete
  product classes, another form of dependency inversion.
- **SRP**: `DesignPatterns/Creational/Builder.cpp` separates step-by-step
  construction from the representation of the built object, keeping each class
  focused on one job, similar in spirit to `01_singleResponsibility.cpp`.
- **LSP**: less directly represented among the current DesignPatterns examples;
  `03_liskovSubstitution.cpp` is the dedicated treatment here.
