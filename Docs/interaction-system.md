# Interaction System

## Purpose

The Interaction System allows the player to detect, focus on, and interact with nearby actors in a flexible and extensible way.

It is designed to:
- Support multiple interactable types
- Remain decoupled from specific actor implementations
- Provide responsive and predictable interaction behavior

---

## Core Component

### `CT_InteractionComponent`

Attached to the player character, this component is responsible for:
- Detecting nearby interactable actors
- Determining which actor is currently in focus
- Triggering interaction events

---

## How It Works

### 1. Candidate Detection (Overlap)

A collision sphere tracks nearby actors.

- On overlap begin → actor is added to candidate list
- On overlap end → actor is removed from candidate list

This maintains a list of **potential interactables**.

---

### 2. Focus Resolution (Trace + Fallback)

Each update cycle:

1. A forward trace checks what the player is looking at
2. If a valid interactable is hit → it becomes the focused actor
3. If not → system falls back to best candidate from overlap list

This ensures:
- Precision when looking directly at objects
- Stability when multiple interactables are nearby

---

### 3. Interaction Execution

When the player presses the interact input:

- The component calls `TryInteract()`
- The focused actor is validated
- Interaction is triggered via a **Blueprint Interface**

---

## Blueprint Interface

### `BPI_CT_Interactable`

All interactable actors implement this interface.

This allows:
- Different actor types (NPCs, objects, puzzles) to respond differently
- Interaction logic to remain **generic and reusable**

Example behaviors:
- Start dialogue
- Open a container
- Trigger an event
- Activate a mechanism

---

## Focus Management

The component tracks a **Focused Actor**:

- Updates when the player looks at a different target
- Can trigger UI updates (e.g., interaction prompts)
- Ensures only one actor is interacted with at a time

---

## Design Decisions

### Why overlap + trace?

Using only a trace:
- Can feel unreliable for small or off-center objects

Using only overlap:
- Lacks precision and intent

Combining both:
- Provides accuracy (trace) + reliability (overlap fallback)

---

### Why use a Blueprint Interface?

- Avoids hard dependencies between systems
- Allows new interactable types without modifying core logic
- Keeps the system extensible and modular

---

### Why maintain a candidate list?

- Prevents expensive repeated searches
- Allows fallback logic when no direct trace hit exists
- Improves responsiveness in dense environments

---

## Edge Cases Handled

- Empty candidate list (no interactables nearby)
- Loss of focus when actor leaves range
- Switching focus between multiple nearby actors
- Interaction attempts with invalid targets

---

## Integration Points

This system integrates with:

- **Dialogue System** → NPC interaction
- **UI System** → interaction prompts
- **Gameplay Tags / Flags** → conditional interaction logic
- **Camera System** → optional focus mode behavior

---

## Future Improvements

- Add interaction priority weighting
- Improve UI feedback for focus changes
- Add contextual interaction prompts
- Support hold interactions or multi-step interactions
- Expand interaction types (e.g., inspect vs use vs talk)