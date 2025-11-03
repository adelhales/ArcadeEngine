// Copyright (c) 2025 Adel Hales

#include "MemoryCard.h"

using namespace MemoryCard;

Game::Game(EngineContext& context) :
    Scene(context)
{
    InitCardPair();
}

void Game::InitCardPair()
{
    cardPair.visibleCooldown.SetDuration(CARD_PAIR_VISIBLE_COOLDOWN_DURATION);
}

void Game::Start()
{
    StartStats();
    StartCards();
}

void Game::StartStats()
{
    stats.level = 0;
}

void Game::StartCards()
{
    EventCardsSpawn();
}

void Game::OnEvent(const sf::Event& event)
{
    if (event.is<sf::Event::MouseButtonPressed>() ||
        event.is<sf::Event::JoystickButtonPressed>())
    {
        if (!IsCardPairSelected())
        {
            EventCardClicked();
        }
    }
}

void Game::Update()
{
    if (IsCardPairSelected() && cardPair.visibleCooldown.IsOver())
    {
        EventCardPairEvaluate();

        if (std::ranges::all_of(cards, [](const Card& card) { return card.found; }))
        {
            stats.level++;
            EventCardsSpawn();
        }
    }
}

void Game::EventCardsSpawn()
{
    std::vector<sf::Color> colors = GenerateCardColors();
    int count = (int)colors.size();

    int rows = (int)std::sqrt(count);
    int cols = (count + rows - 1) / rows;

    cards.clear();
    cards.reserve(count);

    for (int i = 0; i < count; i++)
    {
        sf::Vector2f size = gConfig.windowSize.componentWiseDiv({(float)cols, (float)rows});
        sf::Vector2f position(size.x * (i % cols), size.y * (i / cols));

        EventCardSpawn({position, size}, colors[i]);
    }

    EventCardPairReset();
}

void Game::EventCardSpawn(sf::FloatRect bounds, sf::Color color)
{
    auto& card = cards.emplace_back();

    card.shape.setTexture(ctx.resources.FetchTexture(CARD_TEXTURE_FILENAME));
    card.shape.setFillColor(sf::Color::Transparent);
    card.shape.setOutlineColor(CARD_OUTLINE_COLOR);
    card.shape.setOutlineThickness(-1);
    card.shape.setSize(bounds.size);
    card.shape.setPosition(bounds.position);

    card.color = color;
    card.flipped = false;
    card.found = false;
}

void Game::EventCardClicked()
{
    for (auto& card : cards)
    {
        if (!card.flipped && IsCardHovered(card))
        {
            EventCardFlip(card);
            break;
        }
    }
}

void Game::EventCardFlip(Card& card)
{
    card.shape.setFillColor(card.color);
    card.flipped = true;

    if (!cardPair.first)
    {
        cardPair.first = &card;
    }
    else
    {
        cardPair.second = &card;
        cardPair.visibleCooldown.Restart();
    }
}

void Game::EventCardPairEvaluate()
{
    if (cardPair.first->color == cardPair.second->color)
    {
        cardPair.first->found = true;
        cardPair.second->found = true;
    }
    else
    {
        cardPair.first->shape.setFillColor(sf::Color::Transparent);
        cardPair.second->shape.setFillColor(sf::Color::Transparent);

        cardPair.first->flipped = false;
        cardPair.second->flipped = false;
    }

    EventCardPairReset();
}

void Game::EventCardPairReset()
{
    cardPair.first = nullptr;
    cardPair.second = nullptr;
} 

std::vector<sf::Color> Game::GenerateCardColors() const
{
    std::vector<sf::Color> colors;

    int count = CARDS_COUNT_INITIAL + stats.level * CARDS_COUNT_PER_LEVEL;
    
    colors.reserve(count);

    for (int i = 0; i < count / 2; i++)
    {
        sf::Color color = CARDS_COLORS[i % CARDS_COLORS.size()];
        colors.emplace_back(color);
        colors.emplace_back(color);
    }

    std::ranges::shuffle(colors, std::default_random_engine(std::random_device{}()));

    return colors;
}

bool Game::IsCardHovered(const Card& card) const
{
    return Contains(card.shape, ctx.cursor.GetPosition());
}

bool Game::IsCardPairSelected() const
{
    return cardPair.first && cardPair.second;
}

void Game::Render() const
{
    for (const auto& card : cards)
    {
        ctx.renderer.Draw(card.shape);
    }
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        cardPair.visibleCooldown.Stop();
    }
    else
    {
        cardPair.visibleCooldown.Start();
    }
}