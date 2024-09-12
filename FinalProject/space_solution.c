#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DIST_THRESHOLD 1.0

typedef struct {
    unsigned int last_planet; // Last planet visited
    double distance_traveled; // Distance traveled by the ship
    unsigned int closest_planet; // ID of the closest planet found
    double min_distance; // Minimum distance found
    int jumpNo; //number of hops
} 
ShipState;

ShipAction space_hop(unsigned int crt_planet,
                     unsigned int *connections,
                     int num_connections,
                     double distance_from_mixer,
                     void *ship_state) {
    ShipAction action;

    ShipState *state;
    if (ship_state == NULL) {
        state = malloc(sizeof(ShipState));
        state->last_planet = crt_planet;
    } else {
        state = (ShipState *) ship_state;
    }

    printf("== Space Hopping Menu ==\n");
    printf("1. Check distance from mixer\n");
    printf("2. List connecting planets\n");
    printf("3. Jump to a random planet\n");
    printf("4. Jump to a specific planet by ID\n");
    printf("5. Autopilot(Search algorithm)\n");
    printf("6. Scan Planets(Search algorithm)\n");
    printf("Enter your choice: ");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            //check current planet distance from the mixer
            printf("Distance from mixer: %f\n", distance_from_mixer);
            action.next_planet = crt_planet;
            break;
        case 2:
            //list all connecting planets
            printf("Connecting planets:\n");
            for (int i = 0; i < num_connections; i++) {
                printf("%u\n", connections[i]);
            }
            action.next_planet = crt_planet;
            break;
        case 3:
            //travel to a random planet in the universe
            action.next_planet = RAND_PLANET;
            state->jumpNo++;
            break;
        case 4:
            //travel to specific planet by id
            printf("Enter the ID of the planet to jump to: ");
            unsigned int planet_id;
            scanf("%u", &planet_id);
            action.next_planet = planet_id;
            state->jumpNo++;
            break;

        case 5: {
            //This will iterate through all connecting planets, collecting their distance from mixer and whichever one is
            //closest to mixer will be the next planet to move on to.
            state->closest_planet = crt_planet;
            state->min_distance = distance_from_mixer;

            for (int i = 0; i < num_connections; i++) {
                unsigned int connected_planet = connections[i];
                state->jumpNo++;

                action.next_planet = connected_planet;
                crt_planet = connected_planet;
                printf("Jumped to planet: %u\n", crt_planet);

                // Update ship state to reflect the current planet
                state->last_planet = connected_planet;
                action.ship_state = state;

                double new_distance = distance_from_mixer;

                printf("Distance from mixer here: %f\n", new_distance);

                //returning to the previous planet
                action.next_planet = crt_planet;
                printf("Returned to the previous planet\n");

                if (new_distance < state->min_distance) {
                    state->closest_planet = connected_planet;
                    state->min_distance = new_distance;
                }
            }

            printf("Moving to the closest planet: %u\n", state->closest_planet);

            // Set the next planet to the closest one found in connecting planets.
            state->last_planet = state->closest_planet;
            break;
        }


        case 6: {
            //A selected number of random planets are taken from the universe, whichever one has the smallest distance to
            //mixer will be the planet that will be our starting point.
            ShipState *ship_state;

            ship_state = malloc(sizeof(ShipState));
            ship_state->closest_planet = crt_planet;// Initialize closest to current planet
            ship_state->min_distance = distance_from_mixer;

            for (int i = 0; i < 6; i++) {
                unsigned int random_planet = rand() % num_connections;
                unsigned int connected_planet = connections[random_planet];
                state->jumpNo++;

                //hop to the random planet
                action.next_planet = connected_planet;
                printf("Moved to next planet: %u\n", action.next_planet);

                state->last_planet = connected_planet;

                double new_distance = distance_from_mixer;
                printf("Distance from mixer here: %f\n", new_distance);


                if (new_distance < state->min_distance) {
                    state->closest_planet = action.next_planet;
                    state->min_distance = new_distance;
                }
            }
            ship_state->min_distance = distance_from_mixer;

            printf("Closest planet: %u\n", state->closest_planet);

            action.next_planet = state->closest_planet;
            break;
        }
    }
    state->last_planet = crt_planet;
    action.ship_state = state;
    printf("Number of hops: %d\n", state->jumpNo);
    return action;
}