---
hide:
  - toc
---

# Dialogue Filter Contexts

!!! tip
	These values are available in Lua by their index in the `tes3.dialogueFilterContext` table. For example, `tes3.dialogueFilterContext.serviceBarter`, which has an underlying value that should not be directly used.

Index              | Description
------------------ | -------------------------------------------------------------------------------------------------------------------
clickAnswer        | The context is that a dialogue `choice` option has been selected.
clickTopic         | The context is that a topic has been clicked and is being shown.
clickTopicFallback | If `clickTopic` initially failed, this is what the game used instead. Rarely applicable.
greeting           | The context is that the filter was for a greeting.
hyperlinkParser    | The filtering is performed for parsing hyperlinks.
persuasion         | Used as a result of a persuasion attempt.
script             | The context is from an script, such as from the `:getInfo` method or `tes3.checkMerchantOffersService()`.
serviceBarter      | Applies to the appropriate service.
serviceEnchanting  | Applies to the appropriate service.
serviceRepair      | Applies to the appropriate service.
serviceSpellmaking | Applies to the appropriate service.
serviceSpells      | Applies to the appropriate service.
serviceTraining    | Applies to the appropriate service.
serviceTravel      | Applies to the appropriate service.
topicPopulation    | Performed for each topic that is available to the player.
unknown            | This will never be shown by default, unless a mod makes use of it. It represents that the context is not knowable.
voice              | Applies when a voice greeting is used.
