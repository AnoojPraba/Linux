# Behavioral and Leadership Interview Prep

## STAR format

- **Situation**: brief context - what was going on, why it mattered.
- **Task**: what you specifically were responsible for or trying to
  achieve.
- **Action**: what YOU did (not "we") - the concrete decisions/steps you
  took.
- **Result**: the outcome, ideally with a measurable or concrete impact,
  plus what you learned or would do differently.

## Stories a 15-year candidate should have ready

- A technical disagreement you navigated - what the disagreement was, how
  you argued your position, how it was resolved (including if you were the
  one who ended up wrong, and how you handled that).
- A production incident you led or resolved - the detection, the mitigation
  under pressure, and critically the postmortem/follow-up (what changed
  afterward so it can't recur the same way).
- Mentoring a junior engineer to a specific, concrete outcome (a promotion,
  a skill gap closed, them successfully owning something independently) -
  not just "I mentored someone."
- Pushing back on a deadline for quality/tech-debt reasons - how you framed
  the tradeoff to stakeholders (business terms, not just "the code is
  ugly"), and what happened.
- Influencing a decision cross-team without direct authority - how you
  built the case and got buy-in from people who didn't report to you.

## Worked example: "Tell me about a time you led a production incident"

- **Situation**: "Our payment processing service started throwing
  intermittent 500 errors during peak traffic - about 2% of transactions
  were failing, but only during the evening rush when load was highest."
- **Task**: "As the on-call engineer most familiar with that service, I
  was responsible for diagnosing the root cause and either fixing it or
  mitigating it before it caused a larger customer-facing incident."
- **Action**: "I traced the failures to connection-pool exhaustion, not
  the payment gateway itself - a recent change had a retry-on-timeout
  path that wasn't releasing a connection back to the pool on one error
  branch. Rather than rolling back the whole deploy (which would've also
  reverted two unrelated fixes), I wrote a targeted hotfix for just that
  leak path, tested it against a load-replay of the prior evening's
  traffic in staging, got a second engineer to review it given the time
  pressure, and deployed during a lower-traffic window. I also added a
  temporary alert on connection-pool saturation to catch a recurrence
  faster."
- **Result**: "The error rate dropped to baseline within 15 minutes, with
  no permanent transaction loss. The postmortem led to adding
  connection-pool metrics to every service's standard dashboard, which
  caught a similar issue in a different service two months later before
  it became customer-facing."

This is a strong shape because every sentence has a concrete, specific
decision behind it (why hotfix over rollback, why that reviewer, why that
deploy window) rather than a vague play-by-play - that specificity is
exactly what a follow-up-heavy interviewer is trying to draw out anyway.

## How this round actually goes

- **Format**: usually 30-45 minutes, one-on-one with a manager, peer, or a
  dedicated "bar raiser"-style interviewer. At staff/principal level,
  expect 1-2 dedicated behavioral rounds (sometimes folded into the
  hiring-manager round) rather than a purely technical-only loop.
- **Opening**: a broad prompt ("tell me about a challenging project"),
  with 3-5 minutes of uninterrupted talking room for your initial answer.
- **The follow-up drilling is where it's actually won or lost.** Expect:
  - "What would you have done if the second reviewer wasn't available?"
  - "How did your manager react afterward?"
  - "Was there any disagreement about hotfix vs rollback?"
  - "What did YOU do, versus what did the team do?" (checking you're not
    hiding behind "we")
  - "What would you do differently next time?"
- **What's actually being scored**: ownership (did you drive it, or did
  it happen to you), judgment under pressure (the hotfix-vs-rollback
  tradeoff, escalation timing), technical depth (can you go deep on the
  root cause when pushed), and communication (structured, not rambling).
- **Common failure mode at senior level**: a technically correct but
  team-diffuse answer ("we noticed... we decided... we fixed it") -
  interviewers specifically probe to find your individual contribution,
  so deliberate "I" language matters more than it feels like it should.

## Meta-tip

- At this experience level, interviewers are listening for ownership,
  judgment, and impact - not just a play-by-play of "what happened." Every
  story should make clear what YOU decided, why, and what changed as a
  result.
- Prepare 4-5 flexible stories rather than one story per possible question -
  a good incident story can often be reshaped to answer "tell me about a
  failure," "tell me about working under pressure," or "tell me about a
  time you had to communicate bad news," depending on which angle the
  question asks for.
