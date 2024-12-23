#include "Game.h"
#include "EntityManager.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <cstdlib>  // For rand() and RAND_MAX
#include <ctime>    // For time()

int getRandomColorComponent() 
{
    return rand() % 256;
}

Game::Game(const std::string & config)
{
    init(config);
}

void Game::readConfig(std::string & head, std::ifstream & fin)
{
    if (head == "Window")
    {
        // Window configurations
        int wWidth, wHeight, wFramelimit, wScreenMode;

        fin >> wWidth >> wHeight >> wFramelimit >> wScreenMode;

        // set up default window parameters
        if (wScreenMode == 1) // Assuming 1 represents full screen mode
        {
            m_window.create(sf::VideoMode(wWidth, wHeight), 
                                  "Game Window", 
                                  sf::Style::Fullscreen);
        }
        else // Windowed mode
        {
            m_window.create(sf::VideoMode(wWidth, wHeight), 
                                  "Game Window", 
                                  sf::Style::Default);
        }
        m_window.setFramerateLimit(wFramelimit);
    } else if (head == "Font")
    {
        // Font configurations
        std::string fPath;
        int size, fR, fG, fB;

        fin >> fPath >> size >> fR >> fG >> fB;

        m_font.loadFromFile(fPath);
        m_text.setFont(m_font);
        m_text.setCharacterSize(size);
        m_text.setFillColor(sf::Color(fR, fG, fB));

    } else if (head == "Player")
    {
        fin >> m_playerConfig.SR 
            >> m_playerConfig.CR
            >> m_playerConfig.S 
            >> m_playerConfig.FR 
            >> m_playerConfig.FG 
            >> m_playerConfig.FB 
            >> m_playerConfig.OR 
            >> m_playerConfig.OG 
            >> m_playerConfig.OB 
            >> m_playerConfig.OT 
            >> m_playerConfig.V;
    } else if (head == "Enemy")
    {
        fin >> m_enemyConfig.SR 
            >> m_enemyConfig.CR 
            >> m_enemyConfig.SMIN 
            >> m_enemyConfig.SMAX
            >> m_enemyConfig.OR 
            >> m_enemyConfig.OG 
            >> m_enemyConfig.OB 
            >> m_enemyConfig.OT 
            >> m_enemyConfig.VMIN 
            >> m_enemyConfig.VMAX 
            >> m_enemyConfig.L 
            >> m_enemyConfig.SI;
    } else if (head == "Bullet")
    {
        fin >> m_bulletConfig.SR 
            >> m_bulletConfig.CR
            >> m_bulletConfig.S
            >> m_bulletConfig.FR 
            >> m_bulletConfig.FG 
            >> m_bulletConfig.FB 
            >> m_bulletConfig.OR 
            >> m_bulletConfig.OG 
            >> m_bulletConfig.OB 
            >> m_bulletConfig.OT 
            >> m_bulletConfig.V 
            >> m_bulletConfig.L;
    }
}

void Game::init(const std::string & path)
{
    // Read and initialize with configuration values
    std::ifstream fin(path);

    if (!fin)
    {
        std::cout << "no config file \n";
        return;
    }
    
    std::string tempHead;

    while (fin >> tempHead)
    {
        readConfig(tempHead, fin);
    }

    spawnPlayer();
    m_entities.update();

    // Seed the random number generator at the start of the program
    std::srand(static_cast<unsigned int>(std::time(0)));
}

void Game::run()
{
    // TODO: add pause functionality in here
    //       some systems should function while paused (rendering)
    //       some systems shouldn't (movement / input)
    
    while (m_running)
    {
        if (!m_paused)    
        {
            m_entities.update();

            sEnemySpawner();
            sMovement();
            sLifespan();
            sCollision();
            sUserInput();
            sRender();

            // increment the current frame
            // may need to be moved when pause implemented
            m_currentFrame++;
        } else {
            sUserInput();
            sRender();
        }
    }
}

void Game::setPaused()
{
    m_paused = !m_paused;
}

void Game::spawnPlayer()
{
    // We create every entity by calling EntityManager.addEntity(tag)
    // This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
    auto entity = m_entities.addEntity("player");

    // Entity's transform component using configuration variables
    Vec2 pos = {m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f};
    Vec2 vel = {m_playerConfig.S, m_playerConfig.S};
    entity->cTransform = std::make_shared<CTransform>(pos, vel, 0.0f);
    

    // Entity's shape component using configuration variables
    entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, 
                                              sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
                                              sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
                                              m_playerConfig.OT);

    // Add an input component to the player so that we can use inputs
    entity->cInput = std::make_shared<CInput>();

    // Add the collision component
    entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

    // Since we want this Entity to be our player, set our Game's player variable to be this Entity
    // This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it
    m_player = entity;
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
    // TODO: make sure the enemy is spawned properly with the m_enemyConfig variables
    //       the enemy must be spawned completely within the bounds of the window

    // Randomizing spawning position / speed / shape
    int x_min = 0;
    int x_max = m_window.getSize().x;
    int y_min = 0;
    int y_max = m_window.getSize().y;

    float randpos_x = static_cast<float>(x_min + (rand() % (1 + x_max - x_min)));
    float randpos_y = static_cast<float>(y_min + (rand() % (1 + y_max - y_min)));
    float randSpeed_x = (rand() / (float)RAND_MAX) * (m_enemyConfig.SMAX - m_enemyConfig.SMIN) + m_enemyConfig.SMIN;
    float randSpeed_y = (rand() / (float)RAND_MAX) * (m_enemyConfig.SMAX - m_enemyConfig.SMIN) + m_enemyConfig.SMIN;

    // Randomly decide if the speed should be negative
    if (rand() % 2 == 0) randSpeed_x = -randSpeed_x;
    if (rand() % 2 == 0) randSpeed_y = -randSpeed_y;

    int rand_V = m_enemyConfig.VMIN  + (rand() % (1 + m_enemyConfig.VMAX - m_enemyConfig.VMIN));
    int randR = getRandomColorComponent();
    int randG = getRandomColorComponent();
    int randB = getRandomColorComponent();

    auto entity = m_entities.addEntity("enemy");

    Vec2 pos = {randpos_x, randpos_y};
    Vec2 vel = {randSpeed_x, randSpeed_y};
    entity->cTransform = std::make_shared<CTransform>(pos, vel, 0.0f);

    // Entity's shape component using configuration variables
    entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, rand_V, 
                                            sf::Color(randR, randG, randB),
                                            sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
                                            m_playerConfig.OT);

    entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);

    m_lastEnemySpawnTime = m_currentFrame;
}

// spawns the small enemies when a big one (input entity e) explodes
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    // TODO: spawn small enemies at the location of the input enemy e

    // when we create the smaller enemy, we have to read the values of the original enemy
    // - spawn a number of small enemies equal to the vertices of the original enemy
    // - set each small enemy to the same color as the original, half the size
    // - small enemies are worth double points of the original enemy

    int radius = e->cShape->circle.getRadius();
    size_t se_num = e->cShape->circle.getPointCount();
    for (int i = 1; i <= se_num; i++)
    {
        auto small_enemy = m_entities.addEntity("enemy");

        Vec2 pos = e->cTransform->pos;
        Vec2 vel = e->cTransform->velocity.spin(360.0f / se_num * i);
        small_enemy->cTransform = std::make_shared<CTransform>(pos, vel, 0.0f);

        // Entity's shape component using configuration variables
        small_enemy->cShape = std::make_shared<CShape>(e->cShape->circle.getRadius()/4,
                                                e->cShape->circle.getPointCount(), 
                                                e->cShape->circle.getFillColor(),
                                                e->cShape->circle.getOutlineColor(),
                                                e->cShape->circle.getOutlineThickness());

        small_enemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR/4);

        small_enemy->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
    }
}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & target)
{
    Vec2 start_pos = entity->cTransform->pos;
    
    auto bullet_entity = m_entities.addEntity("bullet");

    Vec2 vel = (start_pos - target);
    vel.normalize();
    vel *= m_bulletConfig.S;

    bullet_entity->cTransform = std::make_shared<CTransform>(start_pos, vel, 0.0f);
    
    // Entity's shape component using configuration variables
    bullet_entity->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, 
                                              sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
                                              sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
                                              m_bulletConfig.OT);

    // Entity's collision component
    bullet_entity->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);

    // Entity's lifespan component
    bullet_entity->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
    Vec2 origin = entity->cTransform->pos;
    for (int i = 1; i <= 64; i++)
    {
        spawnBullet(entity, origin + Vec2(1,0).spin(360.0f / 64 * i));
    }
}

void Game::checkAndReverseVelocity(std::shared_ptr<Entity> entity)
{
    // Get the window size
    float windowWidth = m_window.getSize().x;
    float windowHeight = m_window.getSize().y;

    // Get the entity's position and velocity
    float& posX = entity->cTransform->pos.x;
    float& posY = entity->cTransform->pos.y;
    float& velX = entity->cTransform->velocity.x;
    float& velY = entity->cTransform->velocity.y;

    // Check and reverse velocity if the entity reaches the window boundaries
    if (posX <= 0 || posX >= windowWidth)
    {
        velX = -velX;
    }
    if (posY <= 0 || posY >= windowHeight)
    {
        velY = -velY;
    }
}

// System functions
void Game::sMovement()
{
    // Player movement control system

    // Sample movement speed update
    float dx = 0.0f;
    float dy = 0.0f;

    if (m_player->cInput->up) dy -= 1.0f;
    if (m_player->cInput->down) dy += 1.0f;
    if (m_player->cInput->left) dx -= 1.0f;
    if (m_player->cInput->right) dx += 1.0f;

    // Normalize the direction vector
    float length = std::sqrt(dx * dx + dy * dy);
    if (length != 0.0f) {
        dx = (dx / length) * m_playerConfig.S;
        dy = (dy / length) * m_playerConfig.S;
    }

    // Update the player's position
    m_player->cTransform->pos.x += dx;
    m_player->cTransform->pos.y += dy;

    // Boundary conditions
    float &x = m_player->cTransform->pos.x;
    float &y = m_player->cTransform->pos.y;
    if (x < m_playerConfig.SR) x = m_playerConfig.SR;
    if (x > m_window.getSize().x - m_playerConfig.SR) x = m_window.getSize().x - m_playerConfig.SR;
    if (y < m_playerConfig.SR) y = m_playerConfig.SR;
    if (y > m_window.getSize().y - m_playerConfig.SR) y = m_window.getSize().y - m_playerConfig.SR;

    // Bullet entity movements control
    for (auto& e : m_entities.getEntities("bullet"))
    {
        if (e->isActive())
        {
            e->cTransform->pos.x -= e->cTransform->velocity.x;
            e->cTransform->pos.y -= e->cTransform->velocity.y;
            
            // Destory if Lifespan has reached limit
            if (e->cLifespan->remaining <= 0)
            {
                e->destroy();
            }
        }
    }

    for (auto& e : m_entities.getEntities("enemy"))
    {
        if (e->isActive())
        {
            e->cTransform->pos.x -= e->cTransform->velocity.x;
            e->cTransform->pos.y -= e->cTransform->velocity.y;
            checkAndReverseVelocity(e);
            if (e->cLifespan)
            {
                e->cTransform->angle += 5.0f;
            } else
            {
                e->cTransform->angle += 3.0f;
            }
            
        }
    }
}

void Game::sLifespan()
{
    // for all entities
    //     if entity has no lifespan component, skip it
    //     if entity has > 0 remaining lifespan, subtract 1
    //     if it has lifespan and is alive
    //         scale its alpha channel properly
    //     if it has lifespan and its time is up
    //         destroy the entity
    for (auto& e : m_entities.getEntities())
    {
        if (e->cLifespan)
        {
            if (e->cLifespan->remaining > 0)
            {
                auto lifespan = e->cLifespan;
                
                e->cLifespan->remaining -= 1;
                
                sf::Color fill_color = e->cShape->circle.getFillColor();
                sf::Color outline_color = e->cShape->circle.getOutlineColor();
                fill_color.a = 255 * lifespan->remaining / lifespan->total;
                outline_color.a = 255 * lifespan->remaining / lifespan->total;

                e->cShape->circle.setFillColor(fill_color);
                e->cShape->circle.setOutlineColor(outline_color);
            } else
            {
                e->destroy();
            }
        }
    }

}

void Game::sCollision()
{
    // TODO: implement all proper collisions between entities
    //       be sure to use the collision radius, NOT the shape radius
    for (auto& e : m_entities.getEntities("enemy"))
    {   
        Vec2& enemy_pos = e->cTransform->pos;
        Vec2& player_pos = m_player->cTransform->pos;
        // Player collision event with an enemy resets the position to center
        if (player_pos.dist(enemy_pos) <= e->cCollision->radius + m_player->cCollision->radius)
        {
            if (e->isActive())
            {
                m_player->cTransform->pos.x = m_window.getSize().x / 2.0f;
                m_player->cTransform->pos.y = m_window.getSize().y / 2.0f; 
            }
        }
        
        for (auto& b : m_entities.getEntities("bullet"))
        {
            Vec2& bullet_pos = b->cTransform->pos;
            if (enemy_pos.dist(bullet_pos) <= e->cCollision->radius + b->cCollision->radius)
            {
                if (b->isActive())
                {
                    e->destroy();
                    b->destroy();
                    if (e->cLifespan) 
                    {
                        m_score += 500;
                        break;
                        
                    } else
                    {
                        spawnSmallEnemies(e);
                        m_score += 200;
                        break;
                    }
                }
            }
        }
    }

}

void Game::sEnemySpawner()
{
    // TODO: code which implements enemy spawning should go here
    //
    //       (use m_currentFrame - m_lastEnemySpawnTime) to determine
    //       how long it has been since the last enemy spawned

    if (m_currentFrame - m_lastEnemySpawnTime > m_enemyConfig.SI)
    { 
        spawnEnemy();
    }
}

void Game::sRender()
{
    // TODO: change the code below to draw ALL of the entities
    //       sample drawing of the player Entity that we have created
    m_window.clear();

    // Display the score
    m_text.setFont(m_font); // Ensure the font is set
    m_text.setString("Score: " + std::to_string(m_score));
    m_text.setPosition(10, 10);
    m_window.draw(m_text);

    // set the position of the shape based on the entity's transform->pos
    m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

    // set the rotation of the shape based on the entity's transform->angle
    m_player->cTransform->angle += 1.0f;
    m_player->cShape->circle.setRotation(m_player->cTransform->angle);
    
    m_window.draw(m_player->cShape->circle);

    // set the bullet rendering component
    for (auto& e : m_entities.getEntities("bullet"))
    {
        if (e->isActive())
        {
            e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
            m_window.draw(e->cShape->circle);
        }
    }

    for (auto& e : m_entities.getEntities("enemy"))
    {
        if (e->isActive())
        {
            e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
            e->cShape->circle.setRotation(e->cTransform->angle);
            m_window.draw(e->cShape->circle);
        }
    }

    m_window.display();
}

void Game::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // this event triggers when the window is closed
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }

        // this event is triggered when a key is pressed
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                m_player->cInput->up = true;
                break;
            case sf::Keyboard::S:
                m_player->cInput->down = true;
                break;
            case sf::Keyboard::A:
                m_player->cInput->left = true;
                break;
            case sf::Keyboard::D:
                m_player->cInput->right = true;
                break;
            default:
                break;
            }
        }
        // this event is triggered when a key is released
        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                m_player->cInput->up = false;
                break;
            case sf::Keyboard::S:
                m_player->cInput->down = false;
                break;
            case sf::Keyboard::A:
                m_player->cInput->left = false;
                break;
            case sf::Keyboard::D:
                m_player->cInput->right = false;
                break;
            case sf::Keyboard::P:
                setPaused();
            default:
                break;
            }
        }
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                spawnSpecialWeapon(m_player);
            }
        }
    }
}
