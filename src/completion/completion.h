#ifndef SPLATT_COMPLETE_H
#define SPLATT_COMPLETE_H


/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "../base.h"
#include "../matrix.h"
#include "../sptensor.h"
#include "../timer.h"




/******************************************************************************
 * TYPES
 *****************************************************************************/

typedef enum
{
  SPLATT_TC_SGD,
  SPLATT_TC_GD,
  SPLATT_TC_ALS,
  SPLATT_TC_NALGS
} splatt_tc_type;


typedef struct
{
  idx_t rank;
  idx_t nmodes;
  splatt_tc_type which;

  idx_t dims[MAX_NMODES];
  val_t * factors[MAX_NMODES];
} tc_model;



typedef struct
{
  idx_t nmodes;
  val_t learn_rate;
  idx_t max_its;
  val_t regularization[MAX_NMODES];

  val_t * gradients[MAX_NMODES];

  idx_t nthreads;
  thd_info * thds;

  sp_timer_t train_time;
  sp_timer_t test_time;

  /* GD */
  sp_timer_t grad_time;
  sp_timer_t line_time;
} tc_ws;






/******************************************************************************
 * TENSOR COMPLETION FUNCTIONS
 *****************************************************************************/

void splatt_tc_sgd(
    sptensor_t * train,
    sptensor_t const * const validate,
    tc_model * const model,
    tc_ws * const ws);


void splatt_tc_als(
    sptensor_t * train,
    sptensor_t const * const validate,
    tc_model * const model,
    tc_ws * const ws);


void splatt_tc_gd(
    sptensor_t * train,
    sptensor_t const * const validate,
    tc_model * const model,
    tc_ws * const ws);


/******************************************************************************
 * WORKSPACE FUNCTIONS
 *****************************************************************************/

#define tc_ws_alloc splatt_tc_ws_alloc
/**
* @brief Allocate and initialize a workspace used for tensor completion.
*
* @param model The model we will be computing.
* @param nthreads The number of threads to use during the factorization.
*
* @return The allocated workspace.
*/
tc_ws * tc_ws_alloc(
    tc_model const * const model,
    idx_t nthreads);



#define tc_ws_free splatt_tc_ws_free
/**
* @brief Free the memory allocated for a workspace.
*
* @param ws The workspace to free.
*/
void tc_ws_free(
    tc_ws * ws);




/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/


#define tc_rmse splatt_tc_rmse
/**
* @brief Compute the RMSE against of a factorization against a test tensor.
*        RMSE is defined as: sqrt( tc_loss_sq() / nnz ).
*
* @param test The tensor to test against.
* @param model The factorization the evaluate.
* @param ws Workspace to use (thread buffers are accessed).
*
* @return The RMSE.
*/
val_t tc_rmse(
    sptensor_t const * const test,
    tc_model const * const model,
    tc_ws * const ws);



#define tc_loss_sq splatt_tc_loss_sq
/**
* @brief Compute the sum-of-squared loss of a factorization. Computes:
*           sum((observed-predicted)^2) over all nonzeros in 'test'.
*
* @param test The tensor to test against.
* @param model The factorization the evaluate.
* @param ws Workspace to use (thread buffers are accessed).
*
* @return The loss.
*/
val_t tc_loss_sq(
    sptensor_t const * const test,
    tc_model const * const model,
    tc_ws * const ws);



#define tc_frob_sq splatt_tc_frob_sq
/**
* @brief Compute the sum of the squared Frobenius norms of a model, including
*        regularization penalties.
*
* @param model The factorization to evaluate.
* @param ws Workspace to use (regularization[:] are accessed).
*
* @return \sum_{m=1}^{M} \lambda_m || A^{(m)} ||_F_2.
*/
val_t tc_frob_sq(
    tc_model const * const model,
    tc_ws const * const ws);



#define predict_val splatt_predict_val
/**
* @brief Predict a value of the nonzero in position 'index'.
*
* @param model The model to use for prediction.
* @param test The sparse tensor to test against.
* @param index The index of the nonzero to predict. test->ind[:][index] used.
* @param buffer A buffer at least of size model->rank.
*
* @return The predicted value.
*/
val_t tc_predict_val(
    tc_model const * const model,
    sptensor_t const * const test,
    idx_t const index,
    val_t * const restrict buffer);



#define tc_model_alloc splatt_tc_model_alloc
/**
* @brief Allocate and randomly initialize a model for tensor completion.
*
* @param train The training data, used for dimensionality of the model.
* @param rank The sought rank of the factorization.
* @param which Which factorization algorithm will be used.
*
* @return An allocated model for tensor completion.
*/
tc_model * tc_model_alloc(
    sptensor_t const * const train,
    idx_t const rank,
    splatt_tc_type const which);



#define tc_model_free splatt_tc_model_free
/**
* @brief Free the memory allocated for a tensor completion model.
*
* @param model The model to free.
*/
void tc_model_free(
    tc_model * model);


#endif
