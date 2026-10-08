# Behavioral Question Bank and Story Matrix

Use with `01_BehavioralAndLeadershipInterviewPrep` (STAR + worked example),
`02_PeopleLeadershipHiringAndPerformance`, `03_EstimationRoadmapTradeoffsAndADRs`.
Method: prepare ~8 deep stories, each tagged with multiple themes; practice telling
each in 2 and in 5 minutes; keep facts and numbers accurate (interviewers drill
follow-ups and check references).

## How to build your story matrix
For each story write: **Context** (1-2 sentences), **Your role**, **Action** (3-5
concrete things YOU did - "I", not "we"), **Result** (numbers: latency -40%, $ saved,
incidents halved, time to ship), **Learning**, and **tags**. Then fill the matrix:

| Story (working title) | Conflict | Failure/learning | Leadership/influence | Technical depth | Ambiguity | Customer | Deadline/trade-off | Mentoring |
|---|---|---|---|---|---|---|---|---|
| 1. e.g. Production outage | | | | X | | X | X | |
| 2. ... | | | | | | | | |
(Mark which themes each story can answer. Aim to cover every column with at least
two stories so you never reuse one back-to-back in the same loop.)

## Answer structure and pacing
- **STAR/CAR:** Situation (15%), Task (10%), Action (50-60%), Result + reflection (20%).
- Lead with the point ("I led the migration of X, cutting p99 by 60%") then tell it.
- 2-3 minutes; stop and let them probe. Be ready to go deep on any technical detail,
  alternatives you rejected, who disagreed, what you'd do differently.
- Show the **delta from a mid-level answer:** scope (cross-team/org), ambiguity,
  second-order effects, mentoring others, business impact, written artifacts (RFC/
  ADR/postmortem), long-term mechanisms (not just heroics).

## Question bank by theme (with what is being tested)
### Ownership and impact
1. Tell me about the project you're most proud of. *(scope, impact, ownership)*
2. Describe a time you took ownership of something outside your job description.
3. Tell me about a time you identified a problem nobody asked you to solve.
4. What is the biggest impact you've had on a team's productivity/reliability?
### Technical leadership and judgment
5. Describe a major architectural decision you drove; alternatives and trade-offs. *(ADR/RFC habit)*
6. Tell me about a time you disagreed with a technical direction. How did you resolve it?
7. A time you simplified a system or deleted code/services. *(taste, complexity control)*
8. Describe the hardest bug/incident you debugged. Walk through your method. *(see worked example in 01)*
9. How have you balanced tech debt against delivery? Give a concrete case.
### Conflict and influence
10. A time you influenced a team/leader you had no authority over.
11. A conflict with a peer or manager; what happened and what did you learn?
12. Tell me about pushing back on a deadline or requirements.
13. Describe a time you had to say no to a stakeholder.
### Failure and growth
14. Tell me about your biggest failure/mistake at work. *(accountability, learning, process change)*
15. A time you received critical feedback that was hard to hear.
16. A decision you'd make differently with hindsight.
17. A project that was cancelled or didn't ship; how did you handle it?
### People
18. A time you mentored someone; outcome for them and the team.
19. Handling an underperformer or a difficult team member. *(see 02)*
20. How you built or changed team culture/processes; evidence it worked.
21. Hiring: a hard call, or how you raised the bar.
### Ambiguity, speed and delivery
22. Tell me about a time you had to deliver with unclear requirements.
23. A tight deadline and how you decided what to cut. *(see 03)*
24. Describe a time you changed your mind based on data.
25. A time you learned a new domain/technology quickly to deliver.
### Customer, business and ethics
26. A time you advocated for the customer/user against internal pressure.
27. Connecting engineering work to a business metric.
28. A time you had to deliver bad news to leadership/customers.
29. A situation involving ethics, security, or cutting corners; what did you do?
### Motivation and fit
30. Why are you leaving / what are you looking for next?
31. What kind of environment do you do your best work in? What demotivates you?
32. Where do you want to grow in the next 2-3 years (IC vs management path)?
33. What would your last manager/team say are your strengths and growth areas?
34. Why this company/role? *(research the product, scale, challenges, values)*

## Follow-up probes interviewers use (practice answering each)
"What was YOUR specific contribution?" - "What did the others think?" - "What
alternatives did you consider and why reject them?" - "How did you measure success?" -
"What would you do differently?" - "What was the hardest moment?" - "How did you
know you were right?" - "What if you'd had half the time/people?" - "What happened
afterward - did it stick?"

## Red flags to avoid
Blaming others, "we" with no personal action, no measurable result, rambling
setup, rehearsed-sounding scripts, claiming perfection ("my weakness is working too
hard"), badmouthing employers, fabricated or inflated details, avoiding the failure
question, being unable to discuss the technical specifics of your own story.

## Questions to ask them (shows seniority)
How are technical decisions made and documented? What does success look like in 6-12
months? How does the team handle on-call/incidents and tech debt? What are the biggest
risks to the roadmap? How is performance evaluated at this level? How do teams
collaborate/disagree? What would the first project be?

## Practice plan (1-2 weeks)
1. Write 8 stories in the matrix with real numbers. 2. Rehearse aloud with a timer
and record yourself. 3. Do mock interviews with a peer; have them drill follow-ups.
4. Tailor 3-4 stories to the target company's values/leveling guide. 5. Prepare
2-minute versions of "tell me about yourself", "why this company", "biggest failure".
6. Day-before: review the matrix, sleep, don't memorize scripts - memorize
the key facts and numbers.

## Mini example skeleton ("pushing back on a deadline") - illustrative numbers; replace with YOUR facts
- **S/T:** Team was committed to launching X in 6 weeks for a customer event; my estimate
  of the dependency work was ~10 weeks.
- **A:** (1) Broke down the work and identified the two unknowns; (2) ran a 3-day
  spike on the riskiest one; (3) presented options with trade-offs to product/
  leadership: reduced-scope launch in 6 weeks, full in 10, or +2 engineers with the
  coordination cost; (4) documented the decision in an ADR; (5) set weekly risk
  updates.
- **R:** Shipped the reduced scope on time, full feature 4 weeks later with no sev-1
  incidents; stakeholders adopted range-based estimates for later roadmaps.
- **Learning:** surface the risk the moment it's visible; bring options, not just
  problems.
