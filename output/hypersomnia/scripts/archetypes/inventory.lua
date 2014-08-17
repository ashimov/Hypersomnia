world_archetype_callbacks.INVENTORY = {
	creation = function(self)
		local new_entity = components.create_components {
			inventory = {},
			wield = {},
			item = {}
		}
		
		return new_entity
	end,
	
	construction = function(self, new_entity, is_object_new)
		-- nothing to do if we're recreating
		if not is_object_new then return end
		
		if new_entity.item.is_wielded then
			print "constructing inventory!"
			self.owner_entity_system:post_table("item_wielder_change", {
				wield = true,
				subject = self.object_by_id[new_entity.item.wielder_id],
				item = new_entity,
				wielding_key = components.wield.keys.INVENTORY
			})
		else
			new_entity.item.on_drop(new_entity)
		end
	end
}