/**
 * @file phylib.c
 * @brief C file containing custom functions for a billiards physics simulation library.
 *
 * This C file has all neccesary functions to simulate a billiards enviornment
 * Functions include creating all elements like table and holes, and calculating velocities and acceleration of balls
 */

#include "phylib.h"

/**
 * Creates a new still ball object with the specified number and position.
 * 
 * @param number The number of the still ball.
 * @param pos    A pointer to a phylib_coord structure representing the position of the still ball.
 * @return       A pointer to the newly created still ball object, or NULL if memory allocation fails.
 */
phylib_object *phylib_new_still_ball(unsigned char number, 
phylib_coord *pos) {

    // initializing still ball, and allocating memory
     phylib_object * new_still_ball = (phylib_object *)calloc(1, sizeof(phylib_object));

    // making sure memory allocation worked
    if (new_still_ball == NULL) {
        return NULL;
    }
    
    // assign values to the still ball
    new_still_ball->type = PHYLIB_STILL_BALL;
    new_still_ball->obj.still_ball.number = number;
    new_still_ball->obj.still_ball.pos = *pos;

    return new_still_ball;
}

/**
 * Creates a new rolling ball object with the specified number, position, velocity, and acceleration.
 * 
 * @param number The number of the rolling ball.
 * @param pos    A pointer to a phylib_coord structure representing the position of the rolling ball.
 * @param vel    A pointer to a phylib_coord structure representing the velocity of the rolling ball.
 * @param acc    A pointer to a phylib_coord structure representing the acceleration of the rolling ball.
 * @return       A pointer to the newly created rolling ball object, or NULL if memory allocation fails.
 */
phylib_object *phylib_new_rolling_ball(unsigned char number,
phylib_coord *pos,
phylib_coord *vel,
phylib_coord *acc) {

    // initializing new rolling ball, and allocating memory
    phylib_object *new_rolling_ball = (phylib_object *)calloc(1, sizeof(phylib_object));

    // making sure memory allocation worked
    if (new_rolling_ball == NULL) {
        return NULL;
    }

    // assigning values to rolling ball
    new_rolling_ball->type = PHYLIB_ROLLING_BALL;
    new_rolling_ball->obj.rolling_ball.number = number;
    new_rolling_ball->obj.rolling_ball.pos = *pos;
    new_rolling_ball->obj.rolling_ball.vel = *vel;
    new_rolling_ball->obj.rolling_ball.acc = *acc;

    return new_rolling_ball;

}

/**
 * Creates a new hole object with the specified position.
 * 
 * @param pos A pointer to a phylib_coord structure representing the position of the hole.
 * @return    A pointer to the newly created hole object, or NULL if memory allocation fails.
 */
phylib_object *phylib_new_hole(phylib_coord *pos) {

    // initializing new hole, and allocating memory
    phylib_object * new_hole = (phylib_object *)calloc(1, sizeof(phylib_object));

    // making sure memory allocation worked
    if (new_hole == NULL) {
        return NULL;
    }

    // assigning values to new hole
    new_hole->type = PHYLIB_HOLE;
    new_hole->obj.hole.pos = *pos;

    return new_hole;

}

/**
 * Creates a new horizontal cushion object with the specified y-coordinate.
 * 
 * @param y The y-coordinate of the horizontal cushion.
 * @return  A pointer to the newly created horizontal cushion object, or NULL if memory allocation fails.
 */
phylib_object *phylib_new_hcushion(double y) {
    
    // initializing new h cushion, and allocating memory
    phylib_object * new_hcushion = (phylib_object *)calloc(1, sizeof(phylib_object));

    // making sure memory allocation worked
    if (new_hcushion == NULL) {
        return NULL;
    }

    // assigning values to h cushion
    new_hcushion->type = PHYLIB_HCUSHION;
    new_hcushion->obj.hcushion.y = y;

    return new_hcushion;
}

/**
 * Creates a new vertical cushion object with the specified x-coordinate.
 * 
 * @param x The x-coordinate of the vertical cushion.
 * @return  A pointer to the newly created vertical cushion object, or NULL if memory allocation fails.
 */
phylib_object *phylib_new_vcushion(double x) {

    // initializing new v cushion, and allocating memory
    phylib_object * new_vcushion = (phylib_object *)calloc(1, sizeof(phylib_object));

    // making sure memory allocation worked
    if (new_vcushion == NULL) {
        return NULL;
    }

    // assigning values to v cushion
    new_vcushion->type = PHYLIB_VCUSHION;
    new_vcushion->obj.vcushion.x = x;

    return new_vcushion;
}

/**
 * Creates a new table object with predefined objects such as holes and cushions.
 * 
 * @return A pointer to the newly created table object, or NULL if memory allocation fails.
 */
phylib_table *phylib_new_table(void) {
    
    // initializing coordinates of holes
    phylib_object * top_left = phylib_new_hole(&(phylib_coord){0.0, 0.0});

    phylib_object * top_right = phylib_new_hole(&(phylib_coord){PHYLIB_TABLE_WIDTH, 0.0});

    phylib_object * middle_left = phylib_new_hole(&(phylib_coord){0.0, PHYLIB_TABLE_LENGTH / 2.0});

    phylib_object * bottom_left = phylib_new_hole(&(phylib_coord){0.0, PHYLIB_TABLE_LENGTH});

    phylib_object * middle_right = phylib_new_hole(&(phylib_coord){PHYLIB_TABLE_WIDTH, PHYLIB_TABLE_LENGTH / 2.0});

    phylib_object * bottom_right = phylib_new_hole(&(phylib_coord){PHYLIB_TABLE_WIDTH, PHYLIB_TABLE_LENGTH});

    // initializing new table, and allocating memory
    phylib_table * new_table = (phylib_table *)calloc(1, sizeof(phylib_table));

    // making sure memory allocated worked
    if (new_table == NULL) {
        return NULL;
    }

    // assigning values to table
    new_table->time = 0.0;
    new_table->object[0] = phylib_new_hcushion(0.0);
    new_table->object[1] = phylib_new_hcushion(PHYLIB_TABLE_LENGTH);
    new_table->object[2] = phylib_new_vcushion(0.0);
    new_table->object[3] = phylib_new_vcushion(PHYLIB_TABLE_WIDTH);
    new_table->object[4] = top_left;
    new_table->object[5] = top_right;
    new_table->object[6] = middle_left;
    new_table->object[7] = bottom_left;
    new_table->object[8] = middle_right;
    new_table->object[9] = bottom_right;

    // setting rest of the values to null (placeholders for balls)
    for (int i = 10; i < PHYLIB_MAX_OBJECTS; i++) {
        new_table->object[i] = NULL;
    }

    return new_table;

}

/**
 * Copies an object from the source to the destination.
 * 
 * @param dest A pointer to a pointer to the destination object.
 * @param src  A pointer to a pointer to the source object.
 */
void phylib_copy_object(phylib_object **dest, phylib_object **src) {

    // if src is null, dont proceed
    if (*src == NULL) {
        *dest = NULL;
    } else {
        // allocate memory for dest
        *dest = (phylib_object *)calloc(1, sizeof(phylib_object));
        if (*dest != NULL) {
            // if memory allocation worked, copy memory from our source to dest
            memcpy(*dest, *src, sizeof(phylib_object));
        }
    } 
}

/**
 * Copies a table from the source to the destination.
 * 
 * @param table A pointer to the source table object.
 * @return      A pointer to the newly created copied table object, or NULL if memory allocation fails.
 */
phylib_table *phylib_copy_table(phylib_table *table) {

    // check if table is null before proceeding
    if (table == NULL) {
        return NULL;
    }

    // intialize new table and allocate memory
    phylib_table *new_table = (phylib_table *)calloc(1, sizeof(phylib_table));

    // make sure memory allocation worked
    if (new_table == NULL) {
        return NULL;
    }

    // iterate through our max amount of objects, and copy every object from our old table to new table
    for (int i = 0; i < PHYLIB_MAX_OBJECTS; i++) {
        phylib_copy_object(&new_table->object[i], &table->object[i]);
    }

    // assign time from old table to new table
    new_table->time = table->time;

    return new_table;

}

/**
 * Adds an object to the table.
 * 
 * @param table  A pointer to the table object.
 * @param object A pointer to the object to be added.
 */
void phylib_add_object(phylib_table *table, phylib_object *object) {

    // check if parameters are null before proceeding
    if (object == NULL || table == NULL) {
        return;
    }

    // iterate through objects
    for (int i = 0; i < PHYLIB_MAX_OBJECTS; i++) {
        // check if null, indicating an empty space
        if (table->object[i] == NULL) {
            // assign the object in empty space
            table->object[i] = object;
            return;
        }
    }
}

/**
 * Frees memory associated with the table and its objects.
 * 
 * @param table A pointer to the table object to be freed.
 */
void phylib_free_table(phylib_table *table) {

    // check if table is null before proceeding
    if (table == NULL) {
        return;
    }
    
    // iterate through objects
    for (int i = 0; i < PHYLIB_MAX_OBJECTS; i++) {
        // make sure object isnt null
        if (table->object[i] != NULL) {
            // free object and set to null
            free(table->object[i]);
            table->object[i] = NULL;
        }
    }
    free(table);
}

/**
 * Computes the subtraction of two coordinates.
 * 
 * @param c1 The first coordinate.
 * @param c2 The second coordinate.
 * @return   The result of subtracting c2 from c1.
 */
phylib_coord phylib_sub(phylib_coord c1, phylib_coord c2) {

    // subtract x and y coordinates, creating a new coordinate
    phylib_coord new_coord;
    new_coord.x = c1.x - c2.x;
    new_coord.y = c1.y - c2.y;

    return new_coord;
}

/**
 * Computes the length of a coordinate.
 * 
 * @param c The coordinate.
 * @return  The length of the coordinate.
 */
double phylib_length(phylib_coord c) {

    // compute length
    return sqrt((c.x * c.x) + (c.y * c.y));
}

/**
 * Computes the dot product of two coordinates.
 * 
 * @param a The first coordinate.
 * @param b The second coordinate.
 * @return  The dot product of a and b.
 */
double phylib_dot_product(phylib_coord a, phylib_coord b) {

    // compute dot product
    return (a.x * b.x) + (a.y * b.y);
}

/**
 * Computes the distance between two objects.
 * 
 * @param obj1 The first object.
 * @param obj2 The second object.
 * @return     The distance between obj1 and obj2, or -1 if an error occurs.
 */
double phylib_distance(phylib_object *obj1, phylib_object *obj2) {

    // check if parameters are null before proceeding
    if (obj1 == NULL || obj2 == NULL) {
        return -1;
    }

    // make sure obj1 is a rolling ball before proceeding
    if (obj1->type != PHYLIB_ROLLING_BALL) {
        return -1;
    }

    // compute distances for each case
    if (obj2->type == PHYLIB_ROLLING_BALL) {
        return ((phylib_length(phylib_sub(obj1->obj.rolling_ball.pos, obj2->obj.rolling_ball.pos))) - PHYLIB_BALL_DIAMETER);

    } else if (obj2->type == PHYLIB_STILL_BALL) {
        return ((phylib_length(phylib_sub(obj1->obj.rolling_ball.pos, obj2->obj.still_ball.pos))) - PHYLIB_BALL_DIAMETER);

    } else if (obj2->type == PHYLIB_HOLE) {
        return ((phylib_length(phylib_sub(obj1->obj.rolling_ball.pos, obj2->obj.hole.pos))) - PHYLIB_HOLE_RADIUS);

    } else if (obj2->type == PHYLIB_HCUSHION) {
        return (fabs(obj1->obj.rolling_ball.pos.y - obj2->obj.hcushion.y)) - PHYLIB_BALL_RADIUS;

    } else if (obj2->type == PHYLIB_VCUSHION) {
        return (fabs(obj1->obj.rolling_ball.pos.x - obj2->obj.vcushion.x)) - PHYLIB_BALL_RADIUS;

    } else {
        return -1;
    }

}

/**
 * Updates the position and velocity of a rolling ball after a specified time interval.
 * 
 * @param new  A pointer to the rolling ball object to be updated.
 * @param old  A pointer to the original rolling ball object.
 * @param time The time interval for which the rolling ball is updated.
 */
void phylib_roll(phylib_object *new, phylib_object *old, double time) {

    // null check parameters for proceeding
    if (new == NULL || old == NULL) {
        return;
    }
    
    // make sure parameters are rolling balls before proceeding
    if (new->type != PHYLIB_ROLLING_BALL || old->type != PHYLIB_ROLLING_BALL) {
        return;
    }
    
    // compute x position of our new ball
    new->obj.rolling_ball.pos.x = 
        (old->obj.rolling_ball.pos.x) +
        (old->obj.rolling_ball.vel.x * time) +
        ((0.5) * (old->obj.rolling_ball.acc.x) * (time * time));

    // compute y position of our new ball
    new->obj.rolling_ball.pos.y = 
        (old->obj.rolling_ball.pos.y) +
        (old->obj.rolling_ball.vel.y * time) +
        ((0.5) * (old->obj.rolling_ball.acc.y) * (time * time));

    // compute x velocity of our new ball
    new->obj.rolling_ball.vel.x =
        (old->obj.rolling_ball.vel.x) +
        (old->obj.rolling_ball.acc.x * time);

    // compute y velocity of our new ball
    new->obj.rolling_ball.vel.y =
        (old->obj.rolling_ball.vel.y) +
        (old->obj.rolling_ball.acc.y * time);

    // check if our balls changed direction, if so, set vel and acc to 0 
    if ((new->obj.rolling_ball.vel.x * old->obj.rolling_ball.vel.x) < 0) {
        new->obj.rolling_ball.vel.x = 0.0;
        new->obj.rolling_ball.acc.x = 0.0;
    }

    // ^^^^ same as above
    if ((new->obj.rolling_ball.vel.y * old->obj.rolling_ball.vel.y) < 0) {
        new->obj.rolling_ball.vel.y = 0.0;
        new->obj.rolling_ball.acc.y = 0.0;
    }

}

/**
 * Determines if a rolling ball has stopped moving and converts it to a still ball if so.
 * 
 * @param object A pointer to the rolling ball object.
 * @return       1 if the rolling ball has stopped and converted to a still ball, otherwise 0.
 */
unsigned char phylib_stopped(phylib_object *object) {

    // check if object is null before proceeding
    if (object == NULL) {
        return 0;
    }

    // check if object is rolling ball before proceeding
    if (object->type != PHYLIB_ROLLING_BALL) {
        return 0;
    }

    // if object is going slow enough, change it a still ball
    if (phylib_length(object->obj.rolling_ball.vel) < PHYLIB_VEL_EPSILON) {
        object->type = PHYLIB_STILL_BALL;
        object->obj.still_ball.pos = object->obj.rolling_ball.pos;
        object->obj.still_ball.number = object->obj.rolling_ball.number;
        object->obj.rolling_ball.vel.x = 0.0;
        object->obj.rolling_ball.vel.y = 0.0;
        object->obj.rolling_ball.acc.x = 0.0;
        object->obj.rolling_ball.acc.y = 0.0;
        return 1;
    }
    else {
        return 0;
    }
}
        
/**
 * Performs collision handling between two objects, such as changing direction or deallocating objects.
 * 
 * @param a A pointer to a pointer to the first object.
 * @param b A pointer to a pointer to the second object.
 */
void phylib_bounce(phylib_object **a, phylib_object **b) {

    // do null check on object b
    if (*b == NULL || b == NULL) {
        return;
    }

    // do null check on object a
    if (*a == NULL || b == NULL ) {
        return;
    }

    switch((*b)->type) {

        // change direction of y comp if it hits a horizontal cushion
        case PHYLIB_HCUSHION:
            (*a)->obj.rolling_ball.vel.y = ((*a)->obj.rolling_ball.vel.y) * (-1.0);
            (*a)->obj.rolling_ball.acc.y = ((*a)->obj.rolling_ball.acc.y) * (-1.0);
            break;

        // change direction of x comp if it hits a vertical cushion
        case PHYLIB_VCUSHION: 
            (*a)->obj.rolling_ball.vel.x = ((*a)->obj.rolling_ball.vel.x) * (-1.0);
            (*a)->obj.rolling_ball.acc.x = ((*a)->obj.rolling_ball.acc.x) * (-1.0);
            break;

        // free ball if it lands in hole
        case PHYLIB_HOLE: 
            free(*a);
            *a = NULL;
            break;

        // convert still ball to rolling ball, if still ball gets hit
        case PHYLIB_STILL_BALL: 
            (*b)->type = PHYLIB_ROLLING_BALL;
            (*b)->obj.rolling_ball.vel.x = 0.0;
            (*b)->obj.rolling_ball.vel.y = 0.0;
            (*b)->obj.rolling_ball.acc.x = 0.0;
            (*b)->obj.rolling_ball.acc.y = 0.0;
            (*b)->obj.rolling_ball.number = (*b)->obj.still_ball.number;
            (*b)->obj.rolling_ball.pos = (*b)->obj.still_ball.pos;

        // do necessary math, if 2 rolling balls collide
        case PHYLIB_ROLLING_BALL: {
            phylib_coord r_ab = phylib_sub((*a)->obj.rolling_ball.pos, (*b)->obj.rolling_ball.pos); 
            phylib_coord v_rel = phylib_sub((*a)->obj.rolling_ball.vel, (*b)->obj.rolling_ball.vel);
            phylib_coord n;
            n.x = (r_ab.x) / (phylib_length(r_ab));
            n.y = (r_ab.y) / (phylib_length(r_ab));
            double v_rel_n = phylib_dot_product(v_rel, n);

            (*a)->obj.rolling_ball.vel.x -= (v_rel_n * n.x);
            (*a)->obj.rolling_ball.vel.y -= (v_rel_n * n.y);

            (*b)->obj.rolling_ball.vel.x += (v_rel_n * n.x);
            (*b)->obj.rolling_ball.vel.y += (v_rel_n * n.y);

            double speed_a = phylib_length((*a)->obj.rolling_ball.vel);
            double speed_b = phylib_length((*b)->obj.rolling_ball.vel);

            if (speed_a > PHYLIB_VEL_EPSILON) {
                (*a)->obj.rolling_ball.acc.x = ((((*a)->obj.rolling_ball.vel.x) * (-1.0)) / (speed_a)) * PHYLIB_DRAG;
                (*a)->obj.rolling_ball.acc.y = ((((*a)->obj.rolling_ball.vel.y) * (-1.0)) / (speed_a)) * PHYLIB_DRAG;
            } 
            if (speed_b > PHYLIB_VEL_EPSILON) {
                (*b)->obj.rolling_ball.acc.x = ((((*b)->obj.rolling_ball.vel.x) * (-1.0)) / (speed_b)) * PHYLIB_DRAG;
                (*b)->obj.rolling_ball.acc.y = ((((*b)->obj.rolling_ball.vel.y) * (-1.0)) / (speed_b)) * PHYLIB_DRAG;
            }
            break; }
    }
}

/**
 * Determines the number of rolling balls present on the table.
 * 
 * @param t A pointer to the table object.
 * @return  The count of rolling balls on the table.
 */
unsigned char phylib_rolling(phylib_table *t) {

    // initialize rolling count
    unsigned char rolling_count = 0;

    // null check on table
    if (t == NULL) {
        return rolling_count;
    }

    // check for rolling balls on table
    for (int i = 10; i < PHYLIB_MAX_OBJECTS; i++) {
        if (t->object[i] != NULL) {
            if (t->object[i]->type == PHYLIB_ROLLING_BALL) {

                rolling_count = rolling_count + 1;
            }
        }
    }
    return rolling_count;
}

/**
 * Simulates the physics of the table for a small time segment, updating the positions and velocities of objects accordingly.
 * 
 * @param table A pointer to the table object to be simulated.
 * @return      A pointer to a new table object representing the state after simulation, or NULL if no simulation is possible.
 */
phylib_table *phylib_segment(phylib_table *table) {

    // null check on table
    if (table == NULL) {
        return NULL;
    }
    // check if there are balls on the table, if not we exit
    if (phylib_rolling(table) == 0) {
        return NULL;
    }

    // initalize time to the sim rate
    double time = PHYLIB_SIM_RATE;

    // copy table from the table provided
    phylib_table * new_table = phylib_copy_table(table);

    // loop over time
    while (new_table->time < PHYLIB_MAX_TIME) {

        // loop over balls
        for (int i = 10; i < PHYLIB_MAX_OBJECTS; i++) {
            // roll out every ball
            if (new_table->object[i] != NULL && new_table->object[i]->type == PHYLIB_ROLLING_BALL) {
                phylib_roll(new_table->object[i], table->object[i], time);
            }

        }

            // loop over all objects
            for (int i = 10; i < PHYLIB_MAX_OBJECTS; i++) {

                for (int j = 0; j < PHYLIB_MAX_OBJECTS; j++) {
                    if (new_table->object[j] != NULL && j != i) {
                        
                        // check if 2 objects are close enough for collision
                        if (phylib_distance(new_table->object[i], new_table->object[j]) < 0.0 
                        && phylib_distance(new_table->object[i], new_table->object[j]) != -1) {
                        
                            // do collision measures
                            phylib_bounce(&(new_table->object[i]), &(new_table->object[j]));
                            new_table->time += time;
                            return new_table;
                        }
                    }

                }
                
                if (phylib_stopped(new_table->object[i]) == 1) {
                    new_table->time += time;
                    return new_table;
                }

        } 
        time += PHYLIB_SIM_RATE;
    }      
    new_table->time += time;
    return new_table;
}

/**
 * Generates a string representation of an object for debugging or display purposes.
 * 
 * @param object A pointer to the object.
 * @return       A string representation of the object.
 */
char *phylib_object_string(phylib_object *object) {

    static char string[80];
    if (object==NULL) {
        snprintf( string, 80, "NULL;" );
        return string;
    }

    switch (object->type) {
        case PHYLIB_STILL_BALL:
            snprintf( string, 80,
            "STILL_BALL (%d,%6.1lf,%6.1lf)",
            object->obj.still_ball.number,
            object->obj.still_ball.pos.x,
            object->obj.still_ball.pos.y );
            break;
        case PHYLIB_ROLLING_BALL:
            snprintf( string, 80,
            "ROLLING_BALL (%d,%6.1lf,%6.1lf,%6.1lf,%6.1lf,%6.1lf,%6.1lf)",
            object->obj.rolling_ball.number,
            object->obj.rolling_ball.pos.x,
            object->obj.rolling_ball.pos.y,
            object->obj.rolling_ball.vel.x,
            object->obj.rolling_ball.vel.y,
            object->obj.rolling_ball.acc.x,
            object->obj.rolling_ball.acc.y );
            break;
        case PHYLIB_HOLE:
            snprintf( string, 80,
            "HOLE (%6.1lf,%6.1lf)",
            object->obj.hole.pos.x,
            object->obj.hole.pos.y );
            break;
        case PHYLIB_HCUSHION:
            snprintf( string, 80,
            "HCUSHION (%6.1lf)",
            object->obj.hcushion.y );
            break;
        case PHYLIB_VCUSHION:
            snprintf( string, 80,
            "VCUSHION (%6.1lf)",
            object->obj.vcushion.x );
            break;
    }
    return string;

}
