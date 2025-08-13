# Simple Inventory Subsystem Plugin for Unreal Engine

A lightweight and extensible inventory subsystem for Unreal Engine using **Blueprintable**, **Data-Driven**, and **Subsystem-based** architecture. Ideal for games needing modular inventory logic without relying on actor components or replication by default.

---

## ✨ Features

- ✅ **Subsystem-based architecture** (inherits from `UGameInstanceSubsystem`)
- 📦 **Multiple inventories** supported, identified by `FName`
- 🧱 **Inventory slots and items** handled via `USimpleInventorySlot` and `FInstancedStruct`
- 🔁 **Fully Blueprintable API**
- 🔍 **Access to item counts, slot data, and existence checks**
- 🧹 **Add, remove, and clear items dynamically**
- 🧠 **Optional serialization support** via `FSimpleInventorySubsystemStorage`
- ⚡ **Lightweight and non-replicated by default** (extendable for replication support)

---

## 📁 Folder Structure

Plugins/
└── SimpleInventory/
├── Source/
│ └── SimpleInventory/
│ ├── Public/
│ │ ├── SimpleInventory.h
│ │ ├── SimpleInventorySlot.h
│ │ ├── SimpleInventorySubsystem.h
│ │ └── SimpleInventorySubsystemStorage.h
│ └── Private/
│ │ ├── SimpleInventory.cpp
│ │ ├── SimpleInventorySlot.cpp
│ │ ├── SimpleInventorySubsystem.cpp
│ │ └── SimpleInventorySubsystemStorage.cpp

---

## 🚀 Installation

1. Copy the `SimpleInventory` folder into your project's `Plugins/` directory.
2. Regenerate project files (right-click `.uproject`).
3. Open the project and enable the plugin in **Edit → Plugins**.
4. Recompile your project.

---

## 🛠️ Usage Overview

### 📚 Subsystem Setup

The core class is `USimpleInventorySubsystem`. Access it via Blueprint or C++:

```cpp
USimpleInventorySubsystem* InventorySubsystem = GetGameInstance()->GetSubsystem<USimpleInventorySubsystem>();
```

---

## 📦 Creating an Inventory

```cpp
USimpleInventory* NewInventory;
InventorySubsystem->RegisterInventory(FName("Backpack"), 20, NewInventory);
```

---

## ➕ Adding an Item

```cpp
bool bSuccess;
InventorySubsystem->AddItem("Backpack", ItemStruct, 1, bSuccess);
```

---

## ➖ Removing an Item

```cpp
bool bRemoved;
InventorySubsystem->RemoveItemAtIndex("Backpack", Index, Count, bRemoved);
```

---

## 🔍 Checking Item Existence

```cpp
bool bHasItem;
InventorySubsystem->HasItem("Backpack", ItemID, Count, bHasItem);
```

---

## 🧱 Data Structures

### `USimpleInventorySlot`

Each slot holds a single item and quantity.

### `USimpleInventory`

An inventory object with fixed slot count and basic logic.

### `FSimpleInventorySubsystemStorage`

Optional struct for saving/loading inventories. Supports bulk serialization.

---

## 🤝 Contributing

Feel free to fork, extend, and integrate it into your own systems. If you improve it, consider sharing back via a pull request or issue!

---

## 📄 License

This plugin is released under the MIT License. Feel free to use and modify it in your projects.
