#include "PhysicsEngine.h"

//Constants
const float AIR_RESISTANCE_COEFFICIENT = 0.995;
const float TILE_RESISTANCE_COEFFICIENT = 0.50;
Vector GRAVITY_VECTOR = Vector(0, -0.72);

const int MAX_HORIZONAL_VELOCITY = 20;
const int MAX_VERTICAL_VELOCITY = 18;

PhysicsEngine::PhysicsEngine(EntityManager* entityManager, WorldChunkManager* worldChunkManager)
{
	_entityManager = entityManager;
	_worldChunkManager = worldChunkManager;
}

void PhysicsEngine::updateEntity(Entity* entity)
{
	if (((COMPONENT_DYNAMIC) & entity->getMask()) == (COMPONENT_DYNAMIC))
	{
		dyn = entity->getDynamic();

		//Pos Vector
		targetPos = entity->getPos();

		//Set Base Var
		friction_coeff = AIR_RESISTANCE_COEFFICIENT;
		current_v = entity->getDynamic()->_velocity + GRAVITY_VECTOR;

		//Item Gravitation
		if ((COMPONENT_ITEM & entity->getMask()) == COMPONENT_ITEM)
		{
			player = _entityManager->getPlayer();

			if (!player->getItemContainer()->isFullForItem(entity->getItem()->_primaryID, entity->getItem()->_secondaryID))
			{
				playerXY[0] = playerXY[1] = 0;
				player->getCenterXY(playerXY);

				x_diff = playerXY[0] - targetPos.getX();
				y_diff = playerXY[1] - targetPos.getY();
				manhattenDist = abs(x_diff) + abs(y_diff);

				if (manhattenDist < 100)
				{
					//On collide Add to inventory
					if (manhattenDist < 8)
						_entityManager->addItemToPlayerInventory(entity);
					else
					{
						dir = Vector(x_diff, y_diff);
						Vector::normalize(dir);
						current_v = current_v + (dir.scale((100 - manhattenDist) / (100 * 2)));
					}
				}
			}
		}

		//Limit
		if (current_v.getX() > MAX_HORIZONAL_VELOCITY)
			current_v.setX(MAX_HORIZONAL_VELOCITY);
		if (current_v.getX() < (-MAX_HORIZONAL_VELOCITY))
			current_v.setX(-MAX_HORIZONAL_VELOCITY);

		if (current_v.getY() > MAX_VERTICAL_VELOCITY)
			current_v.setY(MAX_VERTICAL_VELOCITY);
		if (current_v.getY() < (-MAX_VERTICAL_VELOCITY))
			current_v.setY(-MAX_VERTICAL_VELOCITY);

		targetPos = current_v.addTo(targetPos);

		//Entity Collision //TODO

		//Tile Collision
		if (((COMPONENT_AABB | COMPONENT_PHYSICS) & entity->getMask()) == (COMPONENT_AABB | COMPONENT_PHYSICS))
		{
			aabb = entity->getAABB();
			phys = entity->getPhysics();

			//Properties
			exy[0] =  exy[1] = 0;
			entity->getXY(exy);

			ew = aabb->_w;
			eh = aabb->_h;

			//Add Forces
			current_a = Vector::vectorSum(phys->forces).scale(phys->_invMass);

			current_v = (current_v + current_a);
			targetPos = current_a.addTo(targetPos);

			//Limit
			if (current_v.getX() > MAX_HORIZONAL_VELOCITY)
				current_v.setX(MAX_HORIZONAL_VELOCITY);
			if (current_v.getX() < (-MAX_HORIZONAL_VELOCITY))
				current_v.setX(-MAX_HORIZONAL_VELOCITY);

			if (current_v.getY() > MAX_VERTICAL_VELOCITY)
				current_v.setY(MAX_VERTICAL_VELOCITY);
			if (current_v.getY() < (-MAX_VERTICAL_VELOCITY))
				current_v.setY(-MAX_VERTICAL_VELOCITY);

			//Set Target Pos
			velocity_dir = Vector::normalize(current_v);
			velocity_dirFract = Vector((velocity_dir.getX() != 0) ? (1 / velocity_dir.getX()) : 0, (velocity_dir.getY() != 0) ? (1 / velocity_dir.getY()) : 0);

			//No tile collsion
			for (int i = 0; i < 4; i++)
				collisions[i] = false;

			std::list<Tile*> tileCollisionList;
			_worldChunkManager->getTilesInRect((int)targetPos.getX(), (int)targetPos.getY(), ew, eh, tileCollisionList);
			if (tileCollisionList.size() == 0)
			{
				//No tile collsion
				for (int i = 0; i < 4; i++)
					phys->collisions[i] = false;
			}
			else
			{
				//Collides with tiles
				for (Tile* t : tileCollisionList)
				{
					//Get Origin Point
					int originPoint[2] = { exy[0] + (ew / 2), exy[1] - (eh / 2) };

					//Tile Cords
					XY[0] = XY[1] = 0;
					t->getXY(XY);
					lb[0] = XY[0];
					lb[1] = XY[1] - TILE_He;
					rt[0] = XY[0] + TILE_Wi;
					rt[1] = XY[1];

					t1 = (lb[0] - originPoint[0])*velocity_dirFract.getX();
					t2 = (rt[0] - originPoint[0])*velocity_dirFract.getX();
					t3 = (lb[1] - originPoint[1])*velocity_dirFract.getY();
					t4 = (rt[1] - originPoint[1])*velocity_dirFract.getY();

					tmin = max(min(t1, t2), min(t3, t4));
					tmax = min(max(t1, t2), max(t3, t4));

					if (tmax < 0)
						continue;
					else
						tt = tmin;

					if (tt == t1)
					{
						targetPos.setX(targetPos.getX() - ((targetPos.getX() + ew) - XY[0]) - 1);
						if (current_v.getX() > 0)
							current_v.setX(0);
						collisions[1] = true;
					}
					else
					{
						if (tt == t2)
						{
							targetPos.setX(1 + targetPos.getX() + ((XY[0] + TILE_Wi) - targetPos.getX()));
							if (current_v.getX() < 0)
								current_v.setX(0);
							collisions[2] = true;
						}
						else
						{
							if (tt == t4)
							{
								targetPos.setY(ceilf(targetPos.getY() + (XY[1] - (targetPos.getY() - eh))));
								if (current_v.getY() < 0)
									current_v.setY(0);
								collisions[0] = true;
								friction_coeff = TILE_RESISTANCE_COEFFICIENT;
							}
							else
							{
								if (tt == t3)
								{
									targetPos.setY(targetPos.getY() - (targetPos.getY() - (XY[1] - TILE_He)));
									if (current_v.getY() > 0)
										current_v.setY(0);
									collisions[3] = true;
								}
							}
						}
					}
				}
			}

			//Set Collisions
			for (int i = 0; i < 4; i++)
			{
				phys->collisions[i] = collisions[i];
			}

			//Clear Lists
			phys->forces->clear();
		}

		//Friction
		current_v.scale(friction_coeff);
		dyn->_velocity = current_v;
		entity->setPos(targetPos);
	}
}