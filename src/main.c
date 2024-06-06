#include "main.h"

int PORT = 8081;

SDL_Color COLOR_BLACK = {0, 0, 0, 255};
SDL_Color COLOR_WHITE = {255, 255, 255, 255};

TTF_Font *font;
Mix_Music *music;
Mix_Chunk *chompSound;
Mix_Chunk *deathSound;

int main(void) {
  // Initialize SDL2
  init();

  // Create window and renderer
  int windowWidth = GRID_ROW_SIZE * SNAKE_WIDTH;
  SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowWidth, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    fprintf(stderr, "Failed to render window: %s\n", SDL_GetError());
  }



  // Get game mode and join lobby if multiplayer
  enum game_modes game_mode;
  int sock_fd;
  startScreen(renderer, windowWidth, &game_mode, &sock_fd);
  SDL_Delay(250);




  // Initialize game objects
  snake *s = createSnake(50, 50);
  apple *app = createApple(GRID_ROW_SIZE, SNAKE_WIDTH);
  int score = 0;

  // Game Loop
  bool isGameRunning = true;
  SDL_Event event;
  while (isGameRunning) {
    uint32_t startTime = SDL_GetTicks();

    // handle input
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        isGameRunning = false;
      }
      else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_w:
            if (s->direction != Down)
              s->direction = Up;
            break;
          case SDLK_a:
            if (s->direction != Right)
              s->direction = Left;
            break;
          case SDLK_s:
            if (s->direction != Up)
              s->direction = Down;
            break;
          case SDLK_d:
            if (s->direction != Left)
              s->direction = Right;
            break;
        }
      }
    }

    updateSnakePosition(s);

    // Check if snake is colliding with border or with self
    if (checkSnakeBorderCollision(s, GRID_ROW_SIZE) || checkSnakeSelfCollision(s)) {
      isGameRunning = false;
      Mix_PlayChannel(-1, deathSound, 0); // Death sound
    }

    // Check if snake is colliding with apple
    if (checkCollision(getSnakeHead(s), app)) {
      growSnakeBody(s);
      score++;

      Mix_PlayChannel(-1, chompSound, 0);
      moveApple(app, GRID_ROW_SIZE, SNAKE_WIDTH);
    }
    else {
      moveSnake(s);
    }

    // Draw updated objects
    refreshScreen(renderer);
    drawApple(renderer, app);
    drawSnake(renderer, s);
    drawGrid(renderer, windowWidth);
    drawScore(renderer, windowWidth, score);

    SDL_RenderPresent(renderer);

    // Cap Framerate
    uint32_t currTime = SDL_GetTicks();
    float elapsedTime = currTime - startTime;
    SDL_Delay(floor(FRAME_INTERVAL - elapsedTime));
  }

  endScreen(renderer, windowWidth, score);

  // Cleanup
  destroySnake(s);
  destroyApple(app);

  Mix_FreeChunk(chompSound);
  Mix_FreeMusic(music);

  TTF_CloseFont(font);
  TTF_Quit();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}


void startScreen(SDL_Renderer *renderer, int windowWidth, enum game_modes *game_mode, int *sock_fd) {
  // Draw UI
  refreshScreen(renderer);
  drawText(renderer, font, COLOR_WHITE, windowWidth / 2, SNAKE_WIDTH * 2, "Press Any Key to Start!");
  SDL_RenderPresent(renderer);

  // Handle input
  bool isReadyToStart = false;
  SDL_Event event;
  while (!isReadyToStart) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        SDL_Quit();
      }
      else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_s:  // singleplayer
            *game_mode = SinglePlayer;
            isReadyToStart = true;
            break;
          case SDLK_h:  // host
            *game_mode = MultiPlayer;
            hostLobby(renderer, sock_fd);
            isReadyToStart = true;
            break;
          case SDLK_j:  // join
            *game_mode = MultiPlayer;
            joinLobby(renderer, sock_fd);
            isReadyToStart = true;
            break;
        }
      }
    }

    // Cap Framerate
    SDL_Delay(floor(FRAME_INTERVAL));
  }
}

void hostLobby(SDL_Renderer *renderer, int *sock_fd) {
  // Create UI
  ui_element *ui[10] = {0};
  char *hosting = "Hosting Lobby";
  ui[0] = createUIText(renderer, font, COLOR_WHITE, 100, 100, &hosting);
  drawUI(renderer, ui, 1);

  // Host server
  *sock_fd = host(PORT);

  bool isReadyToStart = false;
  bool isLobbyEmpty = true;
  SDL_Event event;
  char buffer[100] = {0};
  while (!isReadyToStart) {
    // Wait for player to join lobby
    if (isLobbyEmpty) {
      if (recv(*sock_fd, buffer, sizeof(buffer), 0) < 0) {
        printf("Player has left the lobby!\n");
      }
      else {
        printf("%s has joined the lobby!\n", buffer);
        isLobbyEmpty = false;
      }
    }

    // handle input
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        SDL_Quit();
      }
      else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_s:  // singleplayer
            isReadyToStart = true;
            char *msg = "Starting";
            send(*sock_fd, msg, 9, 0);
            printf("Sent msg\n");
            break;
        }
      }
    }

    // Cap Framerate
    SDL_Delay(floor(FRAME_INTERVAL));
  }
}

void joinLobby(SDL_Renderer *renderer, int *sock_fd) {
  // Get Host IP
  ui_element *ui[2] = {0};
  char *promptText = "Enter Host:";
  ui[0] = createUIText(renderer, font, COLOR_WHITE, 100, 100, (char **) &promptText);
  drawUI(renderer, ui, 1);

  SDL_StartTextInput();
  char buffer[20] = {0};
  char *buffer_ptr = buffer;
  int idx = 0;
  bool isStillTyping = true;
  while (isStillTyping) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_TEXTINPUT && idx < 19) { // new character entered
        strncpy(buffer + idx, event.text.text, 19 - idx);
        int len = strlen(event.text.text);
        if (idx + len < 19) {
          idx += len;
        }
        else {
          idx = 19;
        }
      }
      else if (event.type == SDL_KEYDOWN) { // backspace
        if (event.key.keysym.sym == SDLK_BACKSPACE && idx > 0) {
          buffer[--idx] = '\0';
        }
        else if (event.key.keysym.sym == SDLK_RETURN) {  // enter
          printf("hit\n");
          buffer[idx] = '\0';
          isStillTyping = false;
        }
      }
    }

    ui[1] = createUIText(renderer, font, COLOR_WHITE, 100, 200, &buffer_ptr);
    drawUI(renderer, ui, 2);
    destroyUIElement(ui[1]);

    SDL_Delay(FRAME_INTERVAL);
  }

  destroyUIElement(ui[0]);
  SDL_StopTextInput();
  


  // Join Host
  *sock_fd = join(buffer, PORT);

  bool hostStarted = false;
  while (!hostStarted) {
    hostStarted = getLobbyStatus(*sock_fd);
    SDL_Delay(floor(FRAME_INTERVAL));
  }
}


void drawScore(SDL_Renderer *renderer, int windowWidth, int score) {
  // Get string of score
  char scoreText[12] = {0};
  sprintf(scoreText, "Score: %d", score);

  drawText(renderer, font, COLOR_WHITE, windowWidth / 2, SNAKE_WIDTH, scoreText);
}

void drawGrid(SDL_Renderer *renderer, int windowWidth) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  
  for (int x = 0; x < windowWidth; x += SNAKE_WIDTH) {
    int y = x;
    SDL_RenderDrawLine(renderer, x, 0, x, windowWidth); // Vertical lines
    SDL_RenderDrawLine(renderer, 0, y, windowWidth, y); // Horizontal lines
  }
  
  // Draw Outer Perimeter of grid
  windowWidth--;  // Set to last visible pixel of window
  SDL_RenderDrawLine(renderer, windowWidth, 0, windowWidth, windowWidth);
  SDL_RenderDrawLine(renderer, 0, windowWidth, windowWidth, windowWidth);
}

void endScreen(SDL_Renderer *renderer, int windowWidth, int score) {
  refreshScreen(renderer);

  // Write "Game Over"
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  drawText(renderer, font, COLOR_WHITE, windowWidth / 2, SNAKE_WIDTH * 2, "Game Over!");

  // Write Final Score
  char scoreText[20] = {0};
  sprintf(scoreText, "Final Score: %d", score);
  drawText(renderer, font, COLOR_WHITE, windowWidth / 2, windowWidth / 2, scoreText);

  SDL_RenderPresent(renderer);

  // Wait for keypress to exit
  bool isReadyToQuit = false;
  SDL_Event event;
  while (!isReadyToQuit) {
    // handle input
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
        isReadyToQuit = true;
      }
    }

    // Cap Framerate
    SDL_Delay(floor(FRAME_INTERVAL));
  }
}

void init() {
  // Video
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL_Init has failed: %s\n", SDL_GetError());
  }

  // Fonts
  if (TTF_Init() < 0) {
    fprintf(stderr, "TTF_Init has failed: %s\n", TTF_GetError());
  }

  font = TTF_OpenFont("./assets/fonts/PixelifySans-VariableFont_wght.ttf", 24);
  if (font == NULL) {
    fprintf(stderr, "Failed to open font: %s\n", TTF_GetError());
  }

  // Audio
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024) < 0) {
    fprintf(stderr, "SDL_Mixer has failed: %s\n", Mix_GetError());
  }

  // Play background music
  music = Mix_LoadMUS("./assets/sounds/music.mp3");
  if (music == NULL) {
    fprintf(stderr, "Failed to load music: %s\n", Mix_GetError());
  }
  else if (Mix_PlayMusic(music, -1) < 0) {
    fprintf(stderr, "Failed to play music: %s\n", Mix_GetError());
  }

  // Load Sound Effects
  chompSound = Mix_LoadWAV("./assets/sounds/chomp.mp3");
  if (chompSound == NULL) {
    fprintf(stderr, "Failed to load sound: %s\n", Mix_GetError());
  }

  deathSound = Mix_LoadWAV("./assets/sounds/death.mp3");
  if (deathSound == NULL) {
    fprintf(stderr, "Failed to load sound: %s\n", Mix_GetError());
  }
}
