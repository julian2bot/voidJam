#include "player.h"
#include "minimap.h"

Player initPlayer(SDL_Renderer *renderer)
{
	SDL_Rect position = {100, 100, 30, 20};
	SDL_FPoint point = {position.x, position.y};
	Player player = {point, position, 90, NULL, .4, FATIGUE_MAX, 0};

	SDL_Texture *rectTexture = getTextureFromImage("voiture.png", renderer);

	player.texture = rectTexture;
	return player;
}

void drawPlayer(SDL_Renderer *renderer, Player player)
{
	SDL_RenderCopyEx(
		renderer,
		player.texture,
		NULL,
		&player.position,
		player.angle,
		NULL,
		SDL_FLIP_NONE);
}

void movePlayer(Player *player)
{
	float rad =  DEG2RAD(player->angle);

	player->pos.x += cosf(rad) * player->vitesse;
	player->pos.y += sinf(rad) * player->vitesse;

	player->position.x = (int)player->pos.x;
	player->position.y = (int)player->pos.y;
}

void updatePlayer(Player *player, int turnLeft, int turnRight,
                  int forward, int back, SDL_Rect *walls, int wall_count, EffectManager *effects)
{
	
	SDL_Rect intersect;
	if(collision(player, walls, wall_count, &intersect))
	{
		// if(player->vitesse > 0) addExplosion(effects, intersect.x + intersect.w / 2, intersect.y + intersect.h / 2);
		printf("Collision\n");
		gameOver(player);
		return;
	}

	updateSteering(player, turnLeft, turnRight, DELTA_TIME);

	player->angle += player->wheelAngle * 1.5f * player->vitesse;


    if (forward)
    {
        player->vitesse += (SPEED_MAX - player->vitesse) * ACCEL;
    }

    if (back)
    {
        player->vitesse -= (player->vitesse - SPEED_MIN) * BRAKE;
    }

    if (!forward && !back)
    {
        player->vitesse *= FRICTION;
    }

    if (player->vitesse > SPEED_MAX)
        player->vitesse = SPEED_MAX;

    if (player->vitesse < SPEED_MIN)
        player->vitesse = SPEED_MIN;

    movePlayer(player);
}

int collision(Player *player, SDL_Rect *walls, int wall_count, SDL_Rect *intersection)
{
	for (int i = 0; i < wall_count; i++)
	{
		if (SDL_IntersectRect(&player->position, &walls[i], intersection))
		{
			return 1;
		}
	}
	return 0;
}

void gameOver(Player *player)
{
	player->vitesse = 0;
	player->tesMort = 1;
}

void destroyPlayer(Player p)
{
	destroy_textures(1, p.texture);
}


// Dashboard
void drawCockPit(SDL_Renderer *renderer, Player player, MusicPlayer* playerUI, SDL_Rect *walls, int wallCount, EffectManager *effects)
{
    drawDashboard(renderer);
    drawSpeedGauge(renderer, player);
    drawFatigueGauge(renderer, player);
    drawSteeringWheel(renderer, player);
    drawMusicPlayer(renderer, playerUI);
    drawMinimap(renderer, player, effects);
}

void drawSteeringWheel(SDL_Renderer *renderer, Player player)
{
    int cx = 150;
    int cy = SCREEN_HEIGHT - 120;

    int outerR = 115;
    int innerR = 105;
    int hubR   = 10;

    float angle = player.wheelAngle * 2.0f;

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    drawRing(renderer, cx, cy, innerR, outerR);
    drawRing(renderer, cx, cy, 0, hubR);

    for (int i = 0; i < 3; i++)
    {
        float a = angle + i * (2.0f * M_PI / 3.0f);

        int x1 = cx + cosf(a) * hubR;
        int y1 = cy + sinf(a) * hubR;

        int x2 = cx + cosf(a) * innerR;
        int y2 = cy + sinf(a) * innerR;

        drawThickLine(renderer, x1, y1, x2, y2, 2);
    }
}


static void drawFatigueGauge(SDL_Renderer *renderer, Player player)
{
    int cx = 390;
    int cy = SCREEN_HEIGHT - 140;

    float ratio = (player.fatigue - FATIGUE_MIN) / (FATIGUE_MAX - FATIGUE_MIN);
    if (ratio < 0) ratio = 0;
    if (ratio > 1) ratio = 1;

    float angle = -120.0f + ratio * 240.0f;

	SDL_SetRenderDrawColor(renderer, 220, 70, 70, 255);
	drawFatigueArc(renderer, cx, cy, 20, 30, -120, -80);

	SDL_SetRenderDrawColor(renderer, 240, 200, 80, 255);
	drawFatigueArc(renderer, cx, cy, 20, 30, -80, 20);

	SDL_SetRenderDrawColor(renderer, 80, 200, 120, 255);
	drawFatigueArc(renderer, cx, cy, 20, 30, 20, 120);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    drawFatigueTicks(renderer, cx, cy);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    drawNeedle(renderer, cx, cy, 15, angle);

    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    drawRing(renderer, cx, cy, 0, 6);
	
	drawText(renderer, NULL, NULL, "Fatigue", cx - 15, cy + 35, 14);
}


static void drawSpeedGauge(SDL_Renderer *renderer, Player player)
{
    int cx = 310;
    int cy = SCREEN_HEIGHT - 80;

    // Ratio vitesse
    float ratio = (player.vitesse - SPEED_MIN) / (SPEED_MAX - SPEED_MIN);
    if (ratio < 0) ratio = 0;
    if (ratio > 1) ratio = 1;

    float angle = -120.0f + ratio * 240.0f;

    // Arc principal
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    drawArc(renderer, cx, cy, 60, -120, 120);

    // Graduations
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    drawSpeedTicks(renderer, cx, cy);

    // Zone rouge (vitesse élevée)
    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
    drawArc(renderer, cx, cy, 60, 80, 120);

    // Aiguille
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    drawNeedle(renderer, cx, cy, 55, angle);

    // Moyeu central
    drawRing(renderer, cx, cy, 0, 5);

	drawText(renderer, NULL, NULL, "Km/h", cx + 5, cy + 5, 11);

}


static void drawNeedle(SDL_Renderer *renderer, int cx, int cy, int length, float angle)
{
    float rad = DEG2RAD(angle);

    int x2 = cx + cosf(rad) * length;
    int y2 = cy + sinf(rad) * length;

    SDL_RenderDrawLine(renderer, cx, cy, x2, y2);
}


static void drawDashboard(SDL_Renderer *renderer)
{
    SDL_Rect dash = {
        0,
        SCREEN_HEIGHT - 180,
        SCREEN_WIDTH,
        180
    };

    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderFillRect(renderer, &dash);
}




void drawThickLine(SDL_Renderer *r, int x1, int y1, int x2, int y2, int thickness)
{
    for (int i = -thickness; i <= thickness; i++)
    {
        SDL_RenderDrawLine(r, x1 + i, y1, x2 + i, y2);
        SDL_RenderDrawLine(r, x1, y1 + i, x2, y2 + i);
    }
}


void updateSteering(Player *player, int left, int right, float dt)
{
    if (left)
        player->wheelVelocity += STEER_FORCE * dt;
    if (right)
        player->wheelVelocity -= STEER_FORCE * dt;

    player->wheelVelocity += -player->wheelAngle * RETURN_FORCE * dt;

    player->wheelVelocity *= expf(-DAMPING * dt);

    player->wheelAngle += player->wheelVelocity * dt;

    if (player->wheelAngle > MAX_WHEEL)
        player->wheelAngle = MAX_WHEEL;
    if (player->wheelAngle < -MAX_WHEEL)
        player->wheelAngle = -MAX_WHEEL;
}


void drawArc(SDL_Renderer *r, int cx, int cy, int radius, float aStart, float aEnd)
{
    int steps = 120;
    for (int i = 0; i < steps; i++)
    {
        float t1 = (float)i / steps;
        float t2 = (float)(i + 1) / steps;

        float a1 = (aStart + t1 * (aEnd - aStart)) * M_PI / 180.0f;
        float a2 = (aStart + t2 * (aEnd - aStart)) * M_PI / 180.0f;

        int x1 = cx + cosf(a1) * radius;
        int y1 = cy + sinf(a1) * radius;
        int x2 = cx + cosf(a2) * radius;
        int y2 = cy + sinf(a2) * radius;

        SDL_RenderDrawLine(r, x1, y1, x2, y2);
    }
}


void drawSpeedTicks(SDL_Renderer *r, int cx, int cy)
{
    float start = -120.0f;
    float end   = 120.0f;

    int smallR1 = 52;
    int smallR2 = 58;
    int bigR1   = 48;
    int bigR2   = 60;

    int tickCount = 20;

    for (int i = 0; i <= tickCount; i++)
    {
        float t = (float)i / tickCount;
        float angle = (start + t * (end - start)) * M_PI / 180.0f;

        int isBig = (i % 5 == 0);

        int r1 = isBig ? bigR1 : smallR1;
        int r2 = isBig ? bigR2 : smallR2;

        int x1 = cx + cosf(angle) * r1;
        int y1 = cy + sinf(angle) * r1;
        int x2 = cx + cosf(angle) * r2;
        int y2 = cy + sinf(angle) * r2;

        SDL_RenderDrawLine(r, x1, y1, x2, y2);
    }
}

void drawFatigueArc(SDL_Renderer *r, int cx, int cy, int r1, int r2, float aStart, float aEnd)
{
    for (int i = 0; i < 360; i++)
    {
        float a = (aStart + (float)i / 360.0f * (aEnd - aStart)) * M_PI / 180.0f;

        int x1 = cx + cosf(a) * r1;
        int y1 = cy + sinf(a) * r1;
        int x2 = cx + cosf(a) * r2;
        int y2 = cy + sinf(a) * r2;

        SDL_RenderDrawLine(r, x1, y1, x2, y2);
    }
}

void drawFatigueTicks(SDL_Renderer *r, int cx, int cy)
{
    float start = -120.0f;
    float end   = 120.0f;

    int r1 = 28;
    int r2 = 32;

    int ticks = 10;

    for (int i = 0; i <= ticks; i++)
    {
        float t = (float)i / ticks;
        float angle = (start + t * (end - start)) * M_PI / 180.0f;

        int x1 = cx + cosf(angle) * r1;
        int y1 = cy + sinf(angle) * r1;
        int x2 = cx + cosf(angle) * r2;
        int y2 = cy + sinf(angle) * r2;

        drawThickLine(r, x1, y1, x2, y2, 2);
    }
}