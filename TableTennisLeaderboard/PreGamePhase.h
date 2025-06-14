#ifndef PRE_GAME_PHASE_H
#define PRE_GAME_PHASE_H

#include "Game.h"
#include "RotaryEncoder.h"
#include "OLED.h"
#include "Players.h"

uint8_t SelectSetLength();
const Player& SelectPlayer(uint8_t player_num, int8_t banned_id = -1);
uint8_t SelectServer(const char* p1_name, const char* p2_name);

/**
 * P1 first selects the set length (first to 6, 11, or 21) using the dial
 * He then sets who's playing using the KY-040 dial (first himself then his opponent)
 * A game object will be created and returned with the inputted data
 * @returns The created Game object that will represent the set and keep track of set data
*/
Game* PreGamePhase()
{
  // Have p1 setup game using OLED and dial
  uint8_t set_length = SelectSetLength(); // 6, 11, or 21
  const Player& p1 = SelectPlayer(1);
  const Player& p2 = SelectPlayer(2, p1.id);
  uint8_t first_server = SelectServer(p1.name, p2.name);

  ShowGameOnMessage();
  delay(1000);
  ClearOLED();

  // Make game object with given data
  Game* game = new Game(set_length, p1, p2, first_server);
  return game;
}

/**
 * P1 will select the set length using the dial
 * @returns Either 6, 11, or 21.
 */
uint8_t SelectSetLength()
{
  uint8_t set_lengths[] = {6, 11, 21};
  int8_t index = 1; // not uint8_t because it can go to -1 before being clamped (set back to 0)
  int8_t prev_index = 1;
  bool last_clk = rotary_encoder_read_clk();

  ShowSelectSetLengthMessage(set_lengths[index]);

  // Loop until player 1 clicks the button on the KY-040 rotary encoder (clicks the dial)
  while (true)
  {
    bool clk_state = rotary_encoder_read_clk();

    // Check for rotation
    // Detect rising edge of CLK only
    if (!last_clk && clk_state)
    {
      if (rotary_encoder_read_dt() != clk_state)
      {
        // Clockwise rotation
        index++;
      }
      else
      {
        // Counter-clockwise rotation
        index--;
      }

      // No wrap-around
      if (index < 0) index = 0;
      if (index > 2) index = 2;

      if (index != prev_index)
      {
        ShowSelectSetLengthMessage(set_lengths[index]);
        prev_index = index;
      }

      delay(100);
    }

    last_clk = clk_state;

    if (rotary_encoder_btn_pressed())
    {
      delay(200); // Crude debounce
      while (rotary_encoder_btn_pressed()); // wait for release
      return set_lengths[index];
    }

    delay(1);
  }
}

/**
 * P1 will select a player using the dial
 * @param player_num Either 1 or 2, representing P1 or P2
 * @param banned_id Default -1. The ID of the player who cannot be selected. Used for when the p1 has already been selected; when choosing p2, it will not be the same as p1 because of this arg
 * @returns The ID of the selected player
 */
const Player& SelectPlayer(uint8_t player_num, int8_t banned_id)
{
  int8_t index = 0; // not uint8_t because it can go to -1 before being clamped (set back to 0)
  int8_t prev_index = 0;
  bool last_clk = rotary_encoder_read_clk();

  if (ALL_SAVED_PLAYERS[0].id == banned_id)
  {
    index = 1;
    prev_index = 1;
  }

  ShowSelectPlayerMessage(player_num, ALL_SAVED_PLAYERS[index]);

  // Loop until player 1 clicks the button on the KY-040 rotary encoder (clicks the dial)
  while (true)
  {
    bool clk_state = rotary_encoder_read_clk();

    // Check for rotation
    // Detect rising edge of CLK only
    if (!last_clk && clk_state)
    {
      bool clockwise_rotation = rotary_encoder_read_dt() != clk_state;
      if (clockwise_rotation)
      {
        // Clockwise rotation
        index++;
      }
      else
      {
        // Counter-clockwise rotation
        index--;
      }

      // No wrap-around (but also check to make sure the banned ID is not selected)
      if (index < 0) index = 0;
      if (index == 0 && ALL_SAVED_PLAYERS[0].id == banned_id) index = 1;
      if (index >= SAVED_PLAYER_COUNT) index = SAVED_PLAYER_COUNT - 1;
      if (index == SAVED_PLAYER_COUNT - 1 && ALL_SAVED_PLAYERS[SAVED_PLAYER_COUNT - 1].id == banned_id) index = SAVED_PLAYER_COUNT - 2;

      if (index != prev_index)
      {
        const Player& ref = ALL_SAVED_PLAYERS[index];
        // Skip over the banned_id. Check to see if index can be pushed forward.
        // Does not work with <=
        if (ref.id == banned_id && clockwise_rotation && index < SAVED_PLAYER_COUNT - 1)
        {
          index++;
        }
        // Does not work with >=
        else if (ref.id == banned_id && !clockwise_rotation && index > 0)
        {
          index--;
        }
        ShowSelectPlayerMessage(player_num, ALL_SAVED_PLAYERS[index]);
        prev_index = index;
      }

      delay(50);
    }

    last_clk = clk_state;

    if (rotary_encoder_btn_pressed())
    {
      delay(200); // Crude debounce
      while (rotary_encoder_btn_pressed()); // wait for release
      return ALL_SAVED_PLAYERS[index];
    }

    delay(1);
  }
}

uint8_t SelectServer(const char* p1_name, const char* p2_name)
{
  int8_t player_num = 1; // not uint8_t because it can go to -1 before being clamped
  int8_t prev_player_num = 1;
  bool last_clk = rotary_encoder_read_clk();

  ShowSelectServerMessage(player_num, p1_name, p2_name);

  // Loop until player 1 clicks the button on the KY-040 rotary encoder (clicks the dial)
  while (true)
  {
    bool clk_state = rotary_encoder_read_clk();

    // Check for rotation
    // Detect rising edge of CLK only
    if (!last_clk && clk_state)
    {
      bool clockwise_rotation = rotary_encoder_read_dt() != clk_state;
      if (clockwise_rotation)
      {
        // Clockwise rotation
        player_num++;
      }
      else
      {
        // Counter-clockwise rotation
        player_num--;
      }

      // No wrap-around
      if (player_num < 1) player_num = 1;
      if (player_num > 2) player_num = 2;

      if (player_num != prev_player_num)
      {
        ShowSelectServerMessage(player_num, p1_name, p2_name);
        prev_player_num = player_num;
      }

      delay(50);
    }

    last_clk = clk_state;

    if (rotary_encoder_btn_pressed())
    {
      delay(200); // Crude debounce
      while (rotary_encoder_btn_pressed()); // wait for release
      return player_num;
    }

    delay(1);
  }
}

#endif