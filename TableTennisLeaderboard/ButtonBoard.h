#ifndef BUTTON_BOARD_H
#define BUTTON_BOARD_H

#define K1 27
#define K2 26
#define K3 25
#define K4 33

void initialize_button_board()
{
  pinMode(K1, INPUT_PULLUP);
  pinMode(K2, INPUT_PULLUP);
  pinMode(K3, INPUT_PULLUP);
  // K4 must have a solder connection issue; it appears to not be properly connected
  // pinMode(K4, INPUT_PULLUP);
}

/**
 * @returns True if the P1 button is currently being pressed
 */
bool P1ButtonPressed()
{
  return digitalRead(K3) == LOW;
}

/**
 * @returns True if the P2 button is currently being pressed
 */
bool P2ButtonPressed()
{
  return digitalRead(K2) == LOW;
}

/**
 * @returns True if the Undo Point button is currently being pressed
 */
bool UndoPointButtonPressed()
{
  return digitalRead(K1) == LOW;
}

/**
 * @returns True if the Reset Game button is currently being pressed
 */
bool ResetGameButtonPressed()
{
  return false; // The black button has some sort of issue. Most not be wired properly. Returning false will just disable it.
  // return digitalRead(K1) == LOW;
}

#endif