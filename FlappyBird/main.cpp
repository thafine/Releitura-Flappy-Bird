#include <raylib.h>
#include <raymath.h>
#include <locale.h>

typedef struct Canos {
    Rectangle rec;
    Texture2D texturaCano;
    float scale;
    Color color;
    bool active;
} Canos;

typedef struct Animal {
    Rectangle texturaRect;
    Texture2D texturaAnimal;// = LoadTexture("personagem.png");
} Animal;
    
int main() {

    const int width = 800;
    const int height = 600;
    
    float X = 0;
    float Y = 300;
    float velocidade = 0;
    bool vivo = true;
    
    Rectangle boxA = { GetScreenWidth()/2.0f - 30, GetScreenHeight()/2.0f - 30, 60, 60 };
    
    Canos cano;
    cano.texturaCano = LoadTexture("cano.png");
    static Canos canos[200] = { 0 };
    static Vector2 canosPos[100] = { 0 };
    static int canosVelocidadeX = 0;
    
    Animal animal;
    animal.texturaAnimal = LoadTexture("personagem.png");
    animal.texturaRect.width = 80;
    animal.texturaRect.height = 180;
    animal.texturaRect.x = X;
    animal.texturaRect.y = Y;
    
    for (int i = 0; i < 100; i++){
        canosPos[i].x = 600 + 380*i; //distancia inicial do tubo para a bola + distancia dos tubos entre si * ?
        canosPos[i].y = -GetRandomValue(0, 150); //posicionamento do y de cima, consequentemente o lugar do buraco dos tubos
    }
            
    int boxASpeedX = 4;
    Rectangle boxCollision = { 0 };
    int screenUpperLimit = 40;
    bool pause = false;
    
    //bool collision = false; 
    bool collisionB = false;
    bool collisionC = false;    
     
    InitWindow(width, height, "Flappy AasdasdaBird");
    SetTargetFPS(60);
 
    //Texture 2D textura = LoadTexture("personagem.png");
    //Rectangle texturaRect = {X, Y, textura.width, textura.height};
    //ultimo parametro eh oq fode o role

    while (!WindowShouldClose()) {
  
        if (!pause) {
            boxA.x += boxASpeedX;
            canosVelocidadeX = 2;
            for (int i = 0; i < 100; i++) canosPos[i].x -=canosVelocidadeX;

            
            for (int i = 0; i < 200; i += 2){
            canos[i].rec.x = canosPos[i/2].x;
            canos[i].rec.y = canosPos[i/2].y;
            canos[i].rec.width = 100; //larguta do tubo de cima
            canos[i].rec.height = 300; //altura tubo de cima

            canos[i+1].rec.x = canosPos[i/2].x;
            canos[i+1].rec.y = 600 + canosPos[i/2].y - 0; //distancia do tubo de baixo começar + buraco - a altura do tubo de baixo
            canos[i+1].rec.width = 100; //larguta do tubo de baixo
            canos[i+1].rec.height = 300; //altura tubo de baixo

            canos[i/2].active = true;
        }
        }
        
        if (((boxA.x + boxA.width) >= GetScreenWidth()) || (boxA.x <= 0)) boxASpeedX *= -1;
        
        boxA.x = GetMouseX() - boxA.width/2;
        boxA.y = GetMouseY() - boxA.height/2;
        
        animal.texturaRect.x = X;
        animal.texturaRect.y = Y;
        /*
        texturaRect.x = posicaoTextura.x;
        texturaRect.y = posicaoTextura.y;
        */
        
        //colisoes
        for (int i = 0; i < 200; i++){
            collisionB = CheckCollisionRecs(animal.texturaRect, canos[i].rec);
            collisionC = CheckCollisionRecs(boxA, canos[i].rec);
            if (collisionB) {
                boxCollision = GetCollisionRec(boxA, canos[i].rec);
                boxCollision = GetCollisionRec(animal.texturaRect, canos[i].rec);
                vivo = false;
            }
            if (collisionC) {
                boxCollision = GetCollisionRec(boxA, canos[i].rec);
                vivo = false;
            }
        }

        if (vivo) {
            // Atualiza a posição do jogador
            Y += velocidade;
            velocidade += 0.4;
        
            // Verifica se as teclas foram pressionadas
            if (IsKeyDown(KEY_SPACE)) {
                velocidade = -7.5;
            }
            if (IsKeyDown(KEY_C)) {
                velocidade = -4;
            }

            // Verifica se o jogador colidiu com o chão ou o teto
            if (Y > 480 || Y < -110) {
                vivo = false;
            }
            

        } else {
            // Reinicia o jogo ao pressionar a tecla R
            if (IsKeyPressed(KEY_R)) {
                X = 40;
                Y = 30;
                velocidade = 0;
                vivo = true;
            }/*
            for (int i = 0; i < 100; i++){
                canosPos[i].x = 600 + 380*i; //distancia inicial do tubo para a bola + distancia dos tubos entre si * ?
                canosPos[i].y = -GetRandomValue(0, 150); //posicionamento do y de cima, consequentemente o lugar do buraco dos tubos
            }*/
        }

        BeginDrawing();

        ClearBackground(BLUE);
        
        DrawRectangleRec(boxA, YELLOW);
        // Criação dos obstaculos do mapa
        for (int i = 0; i < 100; i++) {//quantidade de tubos
            DrawRectangle(canos[i*2].rec.x, canos[i*2].rec.y, canos[i*2].rec.width, canos[i*2].rec.height, GREEN);
            DrawRectangle(canos[i*2 + 1].rec.x, canos[i*2 + 1].rec.y, canos[i*2 + 1].rec.width, canos[i*2 + 1].rec.height, RED);
        }
        
        DrawTextureRec(
            animal.texturaAnimal, 
            (Rectangle){ 0, 0, (float)animal.texturaAnimal.width, (float)animal.texturaAnimal.height },
            Vector2{ X, Y },
            RAYWHITE
        );

        
        if (collisionB || collisionC){
            DrawRectangleRec(boxCollision, LIME);
            DrawText("BATEU!", GetScreenWidth()/2 - MeasureText("BATEU!", 20)/2, screenUpperLimit/2 - 10, 20, BLACK);
            DrawText(TextFormat("Area que bateu: %i", (int)boxCollision.width*(int)boxCollision.height), GetScreenWidth()/2 - 100, screenUpperLimit + 10, 20, BLACK);
            }
         
        // Mostra a mensagem de Game Over
        if (!vivo) {
            DrawText("VOCÊ PERDEU! Aperte R para recomeçar", 100, 380, 20, RED);
        }
        EndDrawing();
    }

    UnloadTexture(animal.texturaAnimal);  // Libera a memória da textura carregada
    CloseWindow();  // Fecha a janela

    return 0;
}
