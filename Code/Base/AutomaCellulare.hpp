#ifndef AUTOMACELLULARE_HPP
#define AUTOMACELLULARE_HPP

#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// Random Engine
std::default_random_engine gen{std::random_device{}()};
std::uniform_real_distribution<> disR(0, 1);

enum class State {
  POOR,
  RICH,
  REVOL,
  REACT,
  ERROR
}; // Different types of people

std::string stateName(State state) { // Return string name of the state
  switch (state) {
  case State::POOR:
    return "POOR";
    break;
  case State::RICH:
    return "RICH";
    break;
  case State::REVOL:
    return "RIVO";
    break;
  case State::REACT:
    return "REAZ";
    break;
  default:
    return "ERROR";
    break;
  }
}

// Declaretion of single cell (Person) class
class Person {
  // Mebmber variables
  State itsState; // State of a Person (E.G. POOR)
  int posX;       // X coordinate on the grid
  int posY;       // Y coordinate on the grid

public:
  Person(State _itsState, int _posX, int _posY)
      : itsState{_itsState}, posX{_posX}, posY{_posY} {}; // Constructor
  // Member functions
  int getX() const { // Get x coord
    return posX;
  }
  int getY() const { // Get y coord
    return posY;
  }
  const State &getState() const { // Return state
    return itsState;
  }
  std::string getStateName() { // Return string name of the state
    switch (itsState) {
    case State::POOR:
      return "POOR";
      break;
    case State::RICH:
      return "\033[1;33mRICH\033[1;0m";
      break;
    case State::REVOL:
      return "\033[1;31mRIVO\033[1;0m";
      break;

    default:
      return "\033[1;34mREAZ\033[1;0m";
      break;
    }
  }

  void changeState(
      State newState) { // Change state checking that the rules are followed
    State oldState{itsState};
    if (itsState == State::POOR && newState == State::REVOL) {
      itsState = newState;
    } else if (itsState == State::REVOL && newState == State::POOR) {
      itsState = newState;
    } else if (itsState == State::RICH && newState == State::REACT) {
      itsState = newState;
    } else if (itsState == State::REACT && newState == State::RICH) {
      itsState = newState;
    } else
      std::cerr << "ERROR CHANGING STATE:" << stateName(oldState) << "->"
                << stateName(newState) << std::endl;
  }
};

class Nation { // Class of the Nation to be simulatetd
  // Stuff
  Person pError{State::ERROR, 0,
                0}; // Erorr type persone used to indicate an error
  // Member variables
  int sideLength; // Length of the side of the grid rappresentig the nation
  double totPop;  // Total Population
  double NR, NP, R, AR; // Different numbers people of different state
  /*
   NR=RICH NP=POOR
   R=REVOL AR=REACT
  */
  double G, g, Ag, beta, alfa; // Probabilites used in simulation
  /*
   G=Capital Gamma
   g=gamma
   ag=bar gamma
  */
  std::vector<Person> Population; // Vector containing population
  // Member functions
  bool isOccupied(int x, int y) {
    bool result{false};
    for (auto it : Population) {
      if (it.getState() != State::ERROR && it.getX() == x && it.getY() == y) {
        result = true;
        break;
      }
    }
    return result;
  }

public:
  Nation(double _NR, double _NP, double _R, double _AR, double _G, double _g,
         double _Ag, double _beta, double _alfa) // Constructor
      : NR{_NR}, NP{_NP}, R{_R}, AR{_AR}, G{_G}, g{_g}, Ag{_Ag}, beta{_beta},
        alfa{_alfa}, Population(NR + NP + R + AR, pError) {
    totPop = NR + NP + R + AR;
    sideLength = static_cast<int>(
        sqrt(totPop)); // Evaluating the side of the grid (USE ONLY SQUARE GRID)
    for (int i{0}; i != NR; ++i) { // Creating RICH People
      int x, y;
      do {
        x = static_cast<int>(disR(gen) *
                             sideLength); // Generating x random coord
        y = static_cast<int>(disR(gen) *
                             sideLength); // Generating y random coord
      } while (isOccupied(x, y)); // Looking for a free space in the grid
      Person newPerson(State::RICH, x, y);
      Population[x * sideLength + y] = newPerson; // Adding the new person
    }
    for (int i{0}; i != NP; ++i) { // Creating POOR People (Same as RICH)
      int x, y;
      do {
        x = static_cast<int>(disR(gen) * sideLength);
        y = static_cast<int>(disR(gen) * sideLength);
      } while (isOccupied(x, y));
      Person newPerson(State::POOR, x, y);
      Population[x * sideLength + y] = newPerson;
      ;
    }
    for (int i{0}; i != R; ++i) { // Creating REVoL People (Same as RICH)
      int x, y;
      do {
        x = static_cast<int>(disR(gen) * sideLength);
        y = static_cast<int>(disR(gen) * sideLength);
      } while (isOccupied(x, y));
      Person newPerson(State::REVOL, x, y);
      Population[x * sideLength + y] = newPerson;
      ;
    }
    for (int i{0}; i != AR; ++i) { // Creating REACT People (Same as RICH)
      int x, y;
      do {
        x = static_cast<int>(disR(gen) * sideLength);
        y = static_cast<int>(disR(gen) * sideLength);
      } while (isOccupied(x, y));
      Person newPerson(State::REACT, x, y);
      Population[x * sideLength + y] = newPerson;
    }
  }

  Person &
  getPerson(double x,
            double y) { // Get person reference from his position in the grid
    for (int i{0}; i != totPop; ++i) {
      if (Population[i].getX() == x && Population[i].getY() == y) {
        return Population[i];
      }
    }
    return pError;
  }

  void print() { // Print the list of people of the Nation
    std::cout << "----------Nation list of people---------\n";
    for (int i{0}; i != totPop; ++i) {
      std::cout << i << ":" << Population[i].getStateName() << std::endl;
    }
    std::cout << "----------------------------------------\n";
  }

  void printGrid() { // Draw the grid
    for (int i{0}; i != sideLength; ++i) {
      for (int j{0}; j != sideLength; j++) {
        std::cout << '|' << this->getPerson(i, j).getStateName() << '|';
      }
      std::cout << std::endl;
    }
  }

  int countNear(double x, double y,
                State stateToFind) { // Count how many people of a state are
                                     // present in a radius R from x,y
    int count{0};
    double center{x * sideLength + y};
    for (int i{-1}; i != 2; ++i) {
      for (int j{-1}; j != 2; ++j) {
        if (x + i >= 0 && x + i < sideLength && y + j >= 0 &&
            y + j < sideLength && center + i + sideLength * j < totPop &&
            center + i + sideLength * j >= 0) {
          if (Population[center + i + sideLength * j].getState() ==
                  stateToFind &&
              (x != Population[center + i + sideLength * j].getX() ||
               y != Population[center + i + sideLength * j].getY())) {
            count++;
          }
        }
      }
    }
    return count;
  }

  void evolve() {
    std::vector<Person> newPopulation = Population;
    for (auto &it : newPopulation) {
      int x{it.getX()}, y{it.getY()};
      switch (it.getState()) {
      case State::POOR:
        if (disR(gen) < g) {
          it.changeState(State::REVOL);
        } else
          for (int i{0}; i != this->countNear(x, y, State::REVOL); ++i) {
            if (disR(gen) < G) {
              it.changeState(State::REVOL);
              break;
            }
          }
        break;
      case State::REVOL:
        for (int i{0}; i != this->countNear(x, y, State::REACT); ++i) {
          if (disR(gen) < Ag) {
            it.changeState(State::POOR);
            break;
          }
        }
        break;
      case State::RICH:
        for (int i{0}; i != this->countNear(x, y, State::REVOL); ++i) {
          if (disR(gen) < alfa) {
            it.changeState(State::REACT);
            break;
          }
        }
        break;
      case State::REACT:
        for (int i{0}; i != this->countNear(x, y, State::POOR); ++i) {
          if (disR(gen) < beta) {
            it.changeState(State::RICH);
            break;
          }
        }
        break;
      default:
        std::cerr << "ERROR: FOUND ERROR STATE DURING EVOLUTION\n";
        break;
      }
    }
    Population = newPopulation;
  }

  double countTotal(State stateToFind) {
    int count{0};
    for (auto it : Population) {
      if (it.getState() == stateToFind) {
        count++;
      }
    }
    return count;
  }
};

#endif