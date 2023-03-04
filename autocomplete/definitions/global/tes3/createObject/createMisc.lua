local miscItemId = "create_misc_item_id"
local miscItemName = "create_misc_item_name"

mwse.log( "creating a misc item of id %s", miscItemId )

local miscItem = tes3.createObject({
	objectType = tes3.objectType.miscItem,
	id = miscItemId,
	name = miscItemName,
	mesh = [[m\misc_dwrv_gear00.nif]]
})
--- @cast miscItem tes3misc

if( miscItem ~= nil ) then
	mwse.log( "misc item created : id is %s", miscItemId )
else
	mwse.log( "failed to create a misc item of id %s", miscItemId )
	return
end

tes3.createReference({
	object = miscItem,
	position = tes3.getPlayerEyePosition(),
	orientation = { 0, 0, 0 },
	cell = tes3.getPlayerCell()
})

mwse.log( "created a reference of a misc item of id %s", miscItem.id )