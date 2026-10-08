# 03_EstimationRoadmapTradeoffsAndADRs

Engineering-leadership scaffolds: estimating under uncertainty, roadmaps and prioritisation, technical debt, trade-off articulation, ADR/RFC templates and metrics, with a senior Q&A.

## Files
- `NOTES.md` - (127 lines) sections: Estimating work; Prioritization and roadmaps; Technical debt; Decision-making and trade-offs; Architecture Decision Records (ADRs) and RFCs; Context      - forces at play: requirements, constraints, scale, team skills; Options      - A) Kafka   B) SQS   C) DB outbox only  (with pros/cons); Decision     - We will use Kafka because ...; Consequences - positive, negative, follow-ups, risks, how we'll know it's wrong; Metrics for engineering leaders; Senior interviewer Q&A (prompts and answer shape)

## How to use this note
- Draft one real ADR from a decision you made (context, options, decision, consequences, how you would know it is wrong) and one estimate with a range and confidence.
- Rehearse the "Senior interviewer Q&A": estimating with unknowns, pushing back on a deadline, tech debt vs features, documenting a hard decision, a failed project, build vs buy, requirements changing midstream.
- Notes are scaffolds: the stories, numbers and outcomes must come from your own real experience; rewrite every example in your voice before an interview.

## Key trade-offs / interview angles
- Estimates: break work into 1-3 day tasks, give ranges and confidence, negotiate scope/resources/time rather than the number.
- Roadmaps: now/next/later with outcomes over feature lists; saying no means acknowledging the goal and showing the trade-off.
- Trade-offs: options x criteria (cost, time, risk, complexity); pre-mortems for risk.
- ADR: short immutable numbered record (Context, Options, Decision, Consequences); RFC/design doc for larger collaborative proposals; writing scales staff-level influence.
- Metrics for engineering leaders.

## Gotchas
- Scaffolds only: example ADR and numbers are illustrative; replace with your own facts.

## Related
- `../01_BehavioralAndLeadershipInterviewPrep`
- `../04_QuestionBankAndStoryMatrix`
- `../../../SystemDesign/topics/17_CapacityEstimationAndBackOfEnvelopeMath`
- `../../../SystemDesign/topics/23_DeploymentStrategies`
- `../../../SystemDesign/topics/18_MicroservicesVsMonolith`

## Conventions when extending
- Keep notes concise and bullet-style; new topic folders use the next two-digit prefix.
- Conceptual only: nothing to build.
