# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with
code in this repository.

## Repository purpose

This is a personal Behavioral / leadership interview-prep repository for a
15+ years-experience candidate - covering the STAR answer format, the stories a
senior candidate should have ready (technical disagreements, production
incidents, mentoring, pushing back on deadlines, cross-team influence without
direct authority), a worked example answer, and how the behavioral interview
round itself typically plays out (format, follow-up drilling, what's actually
being scored). It was split out of the sibling `../SystemDesign/` repo, which
is meant for technical system-design/architecture topics rather than
behavioral/leadership prep. Like `../SystemDesign/`, this material is entirely
conceptual - there is no runnable code, no build system, and no test suite.
Each topic is a `NOTES.md` writeup.

## Structure

- `topics/` - numbered topic folders, each holding a `NOTES.md`:
  `01_BehavioralAndLeadershipInterviewPrep` covers the STAR format, the 5
  story categories a senior candidate should have ready, a fully worked
  example answer (a production-incident story), a breakdown of how the
  behavioral round actually goes (format, follow-up drilling, what's scored),
  and a meta-tip on preparing flexible stories rather than one story per
  possible question.
  `02_PeopleLeadershipHiringAndPerformance` (hiring, onboarding/delegation,
  performance conversations, conflict, leading change, team health),
  `03_EstimationRoadmapTradeoffsAndADRs` (estimating under uncertainty,
  prioritization frameworks, tech debt, build-vs-buy, ADR and RFC templates), and
  `04_QuestionBankAndStoryMatrix` (34 categorized questions, a story-to-theme matrix,
  follow-up probes, red flags, a practice plan). These contain answer SCAFFOLDS - the
  stories and numbers must come from the candidate's real experience.
- Grows over time as more managerial/behavioral topics are added (e.g. team
  conflict resolution, delegation, performance conversations) - not just the
  single starting topic.

## Working with this codebase

- There is no build system, Makefile, or compiler here - this repo is
  conceptual notes only, not code. Nothing under `topics/` is meant to be
  compiled or run.
- Each `NOTES.md` is a concise, bullet-point, interview-focused writeup, not
  essay prose - matching the tone of the sibling repos' `NOTES.md` files.
- New topics should follow the existing naming pattern: a two-digit numeric
  prefix followed by a short descriptive folder name (e.g.
  `02_NextTopicName`).
- Technical system-design/architecture topics remain in the sibling
  `../SystemDesign/` repo; core C/C++/OS-internals topics remain in
  `../C_Basics/`, `../Cpp/`, and `../OS/`. This repo is exclusively for
  behavioral/leadership/managerial interview prep.
