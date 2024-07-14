#include <raylib.h>
#include <raymath.h>
#include <locale.h>

typedef struct Canos {
    Rectangle rec;
    Texture2D texturaCano;
    Color color;
    bool active;
} Canos;

typedef struct Animal {
    Rectangle texturaRect;
    Texture2D texturaAnimal;
} Animal;

int main() {
    const int width = 800;
    const int height = 600;

    float X = 0;
    float Y = 300;
    float velocidade = 0;
    bool vivo = true;

    InitWindow(width, height, "Animal Rocket");
    SetTargetFPS(60);

    Rectangle boxA = { GetScreenWidth()/2.0f - 30, GetScreenHeight()/2.0f - 30, 60, 60 };
    Rectangle boxP = { 80, 180, 250, 110};
    //Rectangle boxPa = { 300, 190, 200, 110};

    Texture2D canoTextura = LoadTexture("canos.png");
    static Canos canos[200] = { 0 };
    static Vector2 canosPos[100] = { 0 };
    static int canosVelocidadeX = 0;

    Animal animal;
    animal.texturaAnimal = LoadTexture("personagem1.png");
    animal.texturaRect.width = 80;
    animal.texturaRect.height = 180;
    animal.texturaRect.x = X;
    animal.texturaRect.y = Y;

    for (int i = 0; i < 100; i++) {
        canosPos[i].x = 600 + 400 * i;
        canosPos[i].y = -GetRandomValue(0, 150);
    }

    Rectangle boxCollision = { 0 };
    int screenUpperLimit = 40;
    bool pause = false;

    bool collisionB = false;
    bool collisionC = false;

    while (!WindowShouldClose()) {

        if (!pause && vivo) {
            canosVelocidadeX = 2;
            for (int i = 0; i < 100; i++) canosPos[i].x -= canosVelocidadeX;

            for (int i = 0; i < 200; i += 2) {
                canos[i].rec.x = canosPos[i / 2].x;
                canos[i].rec.y = canosPos[i / 2].y;
                canos[i].rec.width = 82;
                canos[i].rec.height = 381;

                canos[i + 1].rec.x = canosPos[i / 2].x;
                canos[i + 1].rec.y = 600 + canosPos[i / 2].y;
                canos[i + 1].rec.width = 82;
                canos[i + 1].rec.height = 381;

                canos[i / 2].active = true;
            }
        }
        
        boxA.x = GetMouseX() - boxA.width / 2;
        boxA.y = GetMouseY() - boxA.height / 2;
        boxP.x = X;
        boxP.y = Y;
        // Colisões
        for (int i = 0; i < 200; i++) {
            collisionB = CheckCollisionRecs(boxP, canos[i].rec);
            collisionC = CheckCollisionRecs(boxA, canos[i].rec);
            if (collisionB || collisionC) {
                boxCollision = GetCollisionRec(animal.texturaRect, canos[i].rec);
                vivo = false;
            }
        }

        if (vivo) {
            Y += velocidade;
            velocidade += 0.4;

            if (IsKeyDown(KEY_SPACE)) {
                velocidade = -7.5;
            }
            if (IsKeyDown(KEY_C)) {
                velocidade = -4;
            }

            if (Y > 480 || Y < -110) {
                vivo = false;
            }

        } else {
            if (IsKeyPressed(KEY_R)) {
                X = 40;
                Y = 30;
                velocidade = 0;
                vivo = true;
                for (int i = 0; i < 100; i++) {
                    canosPos[i].x = 600 + 400 * i;
                    canosPos[i].y = -GetRandomValue(0, 150);
                }
            }
        }

        BeginDrawing();

        ClearBackground(BLUE);
        
        DrawRectangleRec(boxA, YELLOW);
        
        for (int i = 0; i < 100; i++) {
            DrawTextureRec(
                canoTextura,
                (Rectangle){ 150, 0, (float)canoTextura.width, -(float)canoTextura.height },  // Inverter a arvore
                (Vector2){ canos[i * 2].rec.x, canos[i * 2].rec.y },
                WHITE
            );
            DrawTextureRec(
                canoTextura,
                (Rectangle){ 150, 0, (float)canoTextura.width, (float)canoTextura.height }, 
                (Vector2){ canos[i * 2 + 1].rec.x, canos[i * 2 + 1].rec.y },
                WHITE
            );
        }


        DrawRectangleRec(boxP, BLUE);   
        DrawTextureRec(
            animal.texturaAnimal,
            (Rectangle){ 0, 0, (float)animal.texturaAnimal.width, (float)animal.texturaAnimal.height },
            (Vector2){ X, Y },
            WHITE
        );
        

        if (collisionB || collisionC) {
            DrawRectangleRec(boxCollision, LIME);
            DrawText("BATEU!", GetScreenWidth() / 2 - MeasureText("BATEU!", 20) / 2, screenUpperLimit / 2 - 10, 20, BLACK);
            DrawText(TextFormat("Area que bateu: %i", (int)boxCollision.width * (int)boxCollision.height), GetScreenWidth() / 2 - 100, screenUpperLimit + 10, 20, BLACK);
        }

        if (!vivo) {
            DrawText("VOCÊ PERDEU! Aperte R para recomeçar", 100, 380, 20, RED);
        }
        EndDrawing();
    }

    UnloadTexture(animal.texturaAnimal);  // Libera a memória da textura carregada
    UnloadTexture(canoTextura);  // Libera a memória da textura do cano
    CloseWindow();  // Fecha a janela

    return 0;
}
