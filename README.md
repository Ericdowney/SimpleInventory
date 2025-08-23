# Simple Inventory

[![Ko-Fi](https://img.shields.io/badge/Ko--fi-F16061?style=for-the-badge&logo=ko-fi&logoColor=white)](https://ko-fi.com/minigamedev)
[![Bluesky](https://img.shields.io/badge/Bluesky-0285FF?style=for-the-badge&logo=Bluesky&logoColor=white)](https://bsky.app/profile/minigamedev.bsky.social)
[![Mastodon](https://img.shields.io/badge/-MASTODON-%232B90D9?style=for-the-badge&logo=mastodon&logoColor=white)](https://mastodon.gamedev.place/@minigamedev)

**SimpleInventory** is an Unreal Engine 5+ plugin that provides a lightweight, extensible inventory system using the Subsystem pattern. The `SimpleInventorySubsystem` is always present during runtime, ensuring your inventory persists across level transitions. You can register and manage multiple named inventories, giving you full flexibility in organizing your game’s inventory systems.


## ✨ Features

* `UGameInstanceSubsystem`: Inventory data persists across levels automatically.
* Multiple named inventories: Organize inventories however you like.
* Blueprint & C++ support: All API functions exposed to Blueprints.
* Stackable items: Built-in logic for stackable / non-stackable items.
* Save & load support: Inventory data stored in a `FSimpleInventorySubsystemStorage` struct for use with `USaveGame`.
* Events for inventory changes: React to additions, removals, clears, and more.
* Detailed logging: Debug inventory behavior with `SimpleInventoryLog`.

## 📦 Installation

* Copy the SimpleInventory plugin folder into your Unreal Engine project's Plugins directory.
* Enable the plugin in Edit → Plugins.
* Restart the editor.

## ⚙️ Setup

### 1. Define Your Item Struct

The plugin provides a base struct:

```c++
USTRUCT(BlueprintType, Blueprintable)
struct SIMPLEINVENTORY_API FSimpleInventoryItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 ID;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool bIsStackable;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 StackSize;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FText ItemName;
};
```

You must subclass this in C++ to add your own game-specific item properties:

```c++
USTRUCT(BlueprintType)
struct FMyGameItem : public FSimpleInventoryItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TObjectPtr<UTexture2D> Icon;

    // Other properties...
};
```

**Note**: Due to Unreal Engine’s Blueprint struct behavior, you must create your struct in C++. Blueprint-created structs will not work because property names cannot be guaranteed.

### 2. Create a Data Table

Once you have your custom struct, you can create a Data Table in the editor to hold your item definitions. I recommend creating a separate data table for each item type in your game. i.e. Gear, weapons, currency, etc.

* In the Content Browser: Right Click → Miscellaneous → Data Table.
* Select your custom struct (i.e., FMyGameItem).
* Fill in rows with item data.

### 3. Accessing the Subsystem

In C++:

```c++
USimpleInventorySubsystem* InventorySubsystem = GetGameInstance()->GetSubsystem<USimpleInventorySubsystem>();
```

In Blueprints:
* Search for Get SimpleInventorySubsystem node.

### 4. Registering an Inventory

```c++
USimpleInventory* MyInventory;
InventorySubsystem->RegisterInventory("PlayerInventory", 30, MyInventory);
```

### 5. Adding Items

Items must be wrapped in an `FInstancedStruct`:

```c++
FMyGameItem ActualItem;
// Fill ActualItem data...
InventorySubsystem->AddItem("PlayerInventory", FInstancedStruct::Make(ActualItem), 5, bResult);
```

**Note**: To avoid using `FInstancedStruct` everywhere, create your own facade (Blueprint Library or custom subsystem) that wraps `SimpleInventorySubsystem` calls and works directly with your game’s item type.

### 6. Saving & Loading

* Use `GetStorage()` to retrieve the current inventory data as an `FSimpleInventorySubsystemStorage`.
* Store it in your `USaveGame` subclass.
* Use `InflateFromStorage()` to restore inventories from saved data.

## 📢 Events

Both `USimpleInventory` and `USimpleInventorySubsystem` broadcast change events when the inventory updates:

* Addition
* Removal
* Multi Removal
* Clear
* Copy
* Force
* Full (inventory is full)

You can bind to:

```c++
InventorySubsystem->OnInventorySubsystemChangeEvent.AddDynamic(this, &ThisClass::OnInventoryChanged);
```

## 🛠 Logging

To enable detailed logging, Add this to your `DefaultEngine.ini`:

```ini
[Core.Log]
SimpleInventoryLog=Verbose
```

---

## 🤝 Contributing

Feel free to fork, extend, and integrate it into your own systems. If you improve it, consider sharing back via a pull request or issue!

---

## 📄 License

This plugin is released under the MIT License. Feel free to use and modify it in your projects.

