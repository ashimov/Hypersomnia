for k, v in pairs(world_archetype_groups.guns) do
	world_archetype_callbacks[v] = {
		creation = function(self)
			local new_entity = components.create_components {
				item = self.owner_scene.weapons[v].item_info,
				weapon = self.owner_scene.weapons[v].weapon_info
			}
			
			new_entity.weapon:create_smoke_group(self.owner_scene.world_object.world)
			
			return new_entity
		end,
		
		construction = function(self, new_entity)
			if new_entity.item.is_owned then
				self.owner_entity_system:post_table("item_ownership", {
					subject = self.object_by_id[new_entity.item.ownership_id],
					item = new_entity,
					pick = true
				})
			else
				new_entity.item.on_drop(new_entity)
			end
		end
	}
end