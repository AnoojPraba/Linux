# Estimation, Roadmaps, Trade-offs and Architecture Decision Records

Senior ICs and tech leads are expected to turn ambiguous goals into plans, say no
with data, and leave a trail of decisions others can follow. Pairs with
`../../../SystemDesign/topics/17_CapacityEstimationAndBackOfEnvelopeMath` (technical
sizing) - this note is about delivery, planning and communication.

## Estimating work
- **Why estimates are wrong:** unknowns (discovery), integration risk, interrupts,
  optimism bias/planning fallacy, scope creep, dependencies, Hofstadter's law.
- **Techniques:** break down (work breakdown structure) until tasks are ~1-3 days;
  **three-point estimates** (optimistic/likely/pessimistic -> PERT (O+4M+P)/6);
  **reference-class forecasting** ("last 3 similar projects took X"); relative
  sizing/story points + measured **velocity/throughput**; spike/prototype to cut
  the biggest unknown first; include testing, rollout, docs, on-call, review latency.
- **Communicate ranges and confidence**, not a single date: "6-9 weeks, 80% by
  week 8; the risks are A and B; we'll re-forecast after the spike". Track
  burn-up charts; re-estimate on new information; tell people EARLY when a date
  will slip (and what you cut/traded).
- Cone of uncertainty: early estimates are +/-4x; shrink by learning, not by
  pressure. Commitments (dates) vs estimates (ranges) vs targets (wishes).
- **Dealing with pressure to cut the estimate:** change the scope, resources, or
  date - not the physics. Offer options: MVP now / full later, cut X, add Y people
  (Brooks' law: adding people to a late project makes it later), drop quality
  (state the risk explicitly).

## Prioritization and roadmaps
- Frameworks: **RICE** (Reach x Impact x Confidence / Effort), **ICE**, **MoSCoW**,
  cost of delay / **WSJF**, impact-effort matrix, **Eisenhower** (urgent vs
  important), **opportunity solution trees**. Use them to structure the
  conversation, not as the answer.
- Balance portfolio: new features, reliability/operability, tech debt, security,
  compliance, learning/innovation - explicit budget (e.g. 20% to debt/KTLO) agreed with
  product so it isn't relitigated each sprint.
- **Roadmap shape:** now/next/later with outcomes (metrics) over feature lists;
  dependencies and risks visible; reviewed quarterly with stakeholders.
- Tie work to business outcomes (revenue, retention, latency, cost, risk reduction).
  Quantify tech-debt/reliability work: incident cost, developer-hours lost, SLO
  burn, security exposure.
- **Saying no / negotiating scope:** acknowledge the goal, show the trade-off with
  data, offer alternatives, agree on what moves; write it down.

## Technical debt
Types: deliberate/prudent (shipped fast with a plan), accidental, bit-rot. Decide
via interest (what it costs per sprint) vs principal (cost to fix). Tactics:
boy-scout rule, strangler-fig migrations, feature flags, refactor with tests/
characterization tests, link to risk metrics, make it visible in the roadmap.

## Decision-making and trade-offs
- Make **reversible** (two-way door) decisions fast and locally; take **one-way**
  doors slowly with more input (Bezos). Disagree and commit once decided.
- **Trade-off articulation:** options -> criteria (cost, time, risk, complexity,
  performance, operability, team skill, lock-in) -> weighted comparison -> decision +
  what would make you revisit. "Build vs buy vs adopt open source" includes total
  cost of ownership (ops, upgrades, security, hiring).
- **Risk management:** pre-mortem ("it's 6 months later and this failed - why?"),
  risk register with likelihood x impact and owners, mitigation vs contingency,
  feature flags/canaries/kill switches, rollout plans, rollback tested.
- Avoid: resume-driven design, second-system effect, premature microservices/
  optimization, analysis paralysis, hero culture.

## Architecture Decision Records (ADRs) and RFCs
- **ADR:** a short, immutable, numbered document capturing ONE architectural
  decision at the time it was made, stored with the code (`docs/adr/0007-use-kafka.md`).
  Template (Michael Nygard):
```
# 7. Use Kafka for order events
Status: Accepted (supersedes ADR-3)   Date: 2026-03-02   Deciders: A, B, C
## Context      - forces at play: requirements, constraints, scale, team skills
## Options      - A) Kafka   B) SQS   C) DB outbox only  (with pros/cons)
## Decision     - We will use Kafka because ...
## Consequences - positive, negative, follow-ups, risks, how we'll know it's wrong
```
  Status lifecycle: Proposed -> Accepted -> Deprecated/Superseded (never edited
  history - add a new ADR). Benefits: onboarding, avoids re-debating, exposes
  assumptions, supports audits.
- **RFC / design doc:** larger, collaborative proposal before building: goals and
  non-goals, background, proposed design, alternatives considered, data/API
  changes, security/privacy, rollout/migration, testing, observability, open
  questions, timeline. Process: draft -> async review with comment deadline ->
  review meeting for contentious points -> approve -> link implementation.
  Principles: short, decision-oriented, alternatives honestly evaluated,
  stakeholders involved EARLY (no surprises), time-boxed.
- **Writing culture:** clear writing scales your influence (staff-level skill):
  one-pagers for executives, tables for trade-offs, diagrams, TL;DR first.

## Metrics for engineering leaders
DORA (deployment frequency, lead time for changes, change failure rate, MTTR),
SLOs/error budgets (`../../../SystemDesign/topics/21_ObservabilityLogsMetricsTraces`),
cycle time, WIP, escaped defects, on-call load, developer-experience surveys.
Goodhart's law: a measure that becomes a target stops being a good measure;
never use them to rank individuals.

## Senior interviewer Q&A (prompts and answer shape)
**Q: How do you estimate a project with lots of unknowns?**
A shape: split into discovery vs delivery; time-boxed spike for the riskiest
unknowns; give a range with confidence and assumptions; re-forecast at milestones;
communicate early about slips and what you can trade.

**Q: Tell me about a time you had to push back on a deadline.**
A shape: goal and context -> your evidence (estimate, risk, dependency) -> options
presented (scope cut / phase / more time / more people with caveats) -> decision ->
outcome and relationship with stakeholders.

**Q: How do you prioritize tech debt versus features?**
A: Quantify interest (incidents, slowed delivery, risk), tie to business outcomes,
agree on a standing budget with product, pick the highest-ROI/highest-risk items,
bundle with feature work touching the same code, track results.

**Q: Describe a hard technical decision and how you documented it.**
A shape: context/constraints -> options with trade-offs -> how you gathered input
(prototype/benchmark/RFC) -> decision (ADR) -> consequences and what you'd revisit;
mention reversibility and what you'd do differently.

**Q: When did a project fail or you made the wrong call?**
A: Own it, explain reasoning at the time (information available), what signals you
missed, the blameless fix, and the concrete process change that came out of it.

**Q: How do you decide build vs buy?**
A: Core differentiator vs commodity, TCO over 3-5 years (build, ops, hiring,
opportunity cost), integration/lock-in/exit strategy, security/compliance, team
skills, time to market, vendor viability; prototype or pilot first.

**Q: How do you keep a plan on track when requirements change midstream?**
A: Make change visible (impact on date/scope), re-prioritize with stakeholders,
keep a rolling short-term plan with a flexible long-term direction, protect the
team from thrash by batching changes, and revisit estimates.
