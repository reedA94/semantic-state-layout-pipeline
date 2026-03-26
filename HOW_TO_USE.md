# How to use these files with a coding AI

## 1. Put the files in the repository root

Copy these files into the root of the repository you want the AI to modify:
- RESEARCH_CONTEXT.md
- TASK_FOR_AI.md
- IMPLEMENTATION_CHECKLIST.md
- PROMPT_TO_AI.txt

Keep your normal project files unchanged.

## 2. Also place supporting material in the repo root if possible

Add these if you have them:
- the paper PDF,
- the current README,
- current summaries/
- current raw_results/

This helps the AI stay aligned with the paper and existing experiment logic.

## 3. Give the AI the whole repository, not snippets

Do not paste isolated source files only.
This task spans:
- method registry,
- benchmark metadata,
- code generation,
- measurement,
- summaries,
- plotting,
- configs.

If the AI sees only fragments, it will likely patch the wrong place or produce inconsistent changes.

## 4. Use PROMPT_TO_AI.txt as the top-level instruction

When opening the coding AI, paste the content of PROMPT_TO_AI.txt as the main prompt.
Then attach or expose the full repository.

## 5. What to ask the AI to return

Require the AI to return all of the following:
- audit of the current repository,
- modified files,
- full contents of changed files,
- exact commands for smoke / medium / full runs,
- short smoke-test evidence,
- explicit list of unfinished items.

## 6. Recommended workflow

### First pass
Ask the AI to implement only the highest-value block:
- full B1–B5 integration,
- compiler matrix,
- B4 profile-shift,
- perf support,
- rollback-aware semantic labeling,
- rollback ablation,
- predicate summaries,
- B5_DAG.

### Second pass
Only after the first pass works, ask for:
- more real benchmark families,
- synthetic sweep,
- extra plotting polish.

## 7. What to reject immediately

Reject the AI output if it:
- changes recognition semantics,
- changes transition semantics,
- changes longest-match behavior,
- removes existing benchmarks,
- ignores rollback-aware experiments,
- claims DAG sharing without implementing it,
- hard-codes only gcc -O3,
- crashes if perf or clang are missing,
- returns prose without actual code changes.

## 8. Minimal prompting sequence

Use this exact order:

1. Put the files in the repo root.
2. Attach the whole repo to the coding AI.
3. Paste the content of PROMPT_TO_AI.txt.
4. Tell the AI: "Do not answer with suggestions only. Modify the repository."
5. After it responds, inspect whether it returned exact commands and actual changed files.

## 9. Best practical setup

Best case:
- a coding AI that can browse the repo,
- edit multiple files,
- run local checks,
- show diffs or full file outputs.

Plain chat-only AI is much less reliable for this task.
