IDIR =include
CC=clang++
CFLAGS=-I$(IDIR) -std=c++17 -O2

ODIR=src/obj
SDIR=src
LDIR =../lib

LIBS=-lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lstdc++fs -lm -lpthread

_DEPS = Core.hpp Timer.hpp Filesys.hpp Camera.hpp Keys.hpp Media.hpp Sound.hpp Render.hpp World.hpp Game.hpp Hitreg.hpp Entity.hpp Vec2.hpp Cmd.hpp Log.hpp Font.hpp Commands.hpp Events.hpp Music.hpp Config.hpp Weapons.hpp Weapon_Class.hpp EntList.hpp HUD.hpp ent/Player.hpp ent/Enemy.hpp ent/Bullet.hpp ent/Bullet.hpp ent/Medpack.hpp ent/Spawner.hpp br/BRUSH_PUSH.hpp br/BRUSH_HURT.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o Core.o Timer.o Core_Console.o Media.o Sound.o Filesys.o Camera.o Keys.o Render.o World.o Game.o Hitreg.o Entity.o Vec2.o Cmd.o Log.o Font.o Commands.o Events.o Music.o Config.o Weapons.o HUD.o ent/Player.o ent/Medpack.o ent/Enemy.o ent/Bullet.o ent/Spawner.o ent/Explosion.o br/BRUSH_PUSH.o br/BRUSH_HURT.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

OUTPUT = quakem

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -MD -MP -c -o $@ $<
make: $(OBJ)
	$(CC) -o $(OUTPUT) $^ $(CFLAGS) $(LIBS)
		
debug: $(OBJ)
	$(CC) -g -o $(OUTPUT) $^ $(CFLAGS) $(LIBS) 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

-include $(OBJ:.o=.d)
