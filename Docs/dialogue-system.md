# Dialogue System

## Purpose

The Dialogue System provides a flexible, data-driven framework for handling conversations, branching dialogue, and condition-based narrative flow.

It is designed to:
- Support conditional dialogue based on game state
- Allow branching paths without hardcoding logic
- Integrate with time, flags, and other systems
- Remain scalable for future narrative complexity

---

## Core Components

### Dialogue Data Asset (`CT_DialogueDataAsset`)

Dialogue is defined using data assets that contain:
- Dialogue nodes (lines of text)
- Conditions for each node
- Links to next nodes or branches
- Optional fallback behavior

This allows dialogue to be authored without modifying core logic.

---

### Dialogue Subsystem (`CT_DialogueSubsystem`)

The Dialogue Subsystem is responsible for:
- Starting and managing dialogue sessions
- Evaluating conditions for dialogue nodes
- Determining which node should be displayed
- Tracking dialogue progression

It acts as the central controller for dialogue flow.

---

### Dialogue Component

Attached to NPCs or interactable actors, this component:
- Initiates dialogue when interacted with
- Passes the appropriate dialogue asset to the subsystem

---

## How It Works

### 1. Interaction Triggers Dialogue

- Player interacts with an NPC or object
- Interaction Component calls the Dialogue Component
- Dialogue Component sends a dialogue asset to the Dialogue Subsystem

---

### 2. Dialogue Starts

The Dialogue Subsystem:
- Loads the dialogue asset
- Selects the starting node
- Displays the first line

---

### 3. Condition Evaluation

Each dialogue node may have conditions such as:
- Time block (e.g., Dusk, Night)
- Gameplay flags (quest state, previous choices)
- Other contextual checks

The system:
- Evaluates all conditions for a node
- Determines if the node is valid
- Falls back to a default node if conditions fail

---

### 4. Branching Logic

Dialogue can branch based on:
- Player choices
- Game state (flags, time, etc.)

Each node defines:
- Possible next nodes
- Conditions for each branch

---

### 5. Progression

As the player advances dialogue:
- The subsystem determines the next valid node
- The UI updates with the new line
- State may be updated (e.g., flags set)

---

## Condition System

Conditions are evaluated using shared systems:

- **Time Subsystem** → time block checks
- **Flag Subsystem** → gameplay state checks

This keeps dialogue logic:
- Centralized
- Reusable
- Easy to extend

---

## Fallback Behavior

If no valid node is found due to failing conditions:
- A fallback node is used

This prevents:
- Broken dialogue flows
- Dead ends caused by missing conditions

---

## Design Decisions

### Why data-driven dialogue?

- Separates content from logic
- Allows rapid iteration without recompiling
- Makes dialogue easier to scale and maintain

---

### Why use a subsystem?

- Centralizes dialogue control
- Avoids duplicating logic across actors
- Ensures consistent behavior across all dialogue interactions

---

### Why condition-based nodes?

- Enables dynamic world responses
- Allows dialogue to reflect player choices and progression
- Supports time-based and state-based storytelling

---

## Edge Cases Handled

- Missing or invalid dialogue nodes
- Failed condition checks
- Fallback node usage
- Dialogue interruption or exit
- Re-entering dialogue after state changes

---

## Integration Points

The Dialogue System integrates with:

- **Interaction System** → triggers dialogue
- **Time System** → time-based dialogue variation
- **Flag System** → narrative progression and branching
- **UI System** → dialogue display and player input

---

## Future Improvements

- Player dialogue choices with consequences
- Dialogue history tracking
- Voice/audio integration support
- More advanced condition types (e.g., relationship states)
- Dialogue event hooks (trigger gameplay events mid-conversation)