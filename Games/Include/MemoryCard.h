// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/MemoryCardTypes.h"

namespace MemoryCard
{
    class Game : public Scene
    {
    private:
        std::vector<Card> cards;
        CardPair cardPair;
        Stats stats;

    public:
        Game(EngineContext&);

        void Start();
        void OnEvent(const sf::Event&);
        void Update();
        void Render() const;
        void OnPause(bool);

    private:
        void InitCardPair();

        void StartStats();
        void StartCards();

        void EventCardsSpawn();
        void EventCardSpawn(sf::FloatRect bounds, sf::Color color);
        void EventCardClicked();
        void EventCardFlip(Card& card);
        void EventCardPairEvaluate();
        void EventCardPairReset();

        std::vector<sf::Color> GenerateCardColors() const;
        bool IsCardHovered(const Card& card) const;
        bool IsCardPairSelected() const;
    };
}