#include "Job.h"

Job::Job() : idJob(0), toolSetId(0), toolSetSize(0), toolCoust() {}

Job::Job(int idJob, int toolSetId, int toolSetSize, int toolCoust)
	: idJob(idJob), toolSetId(toolSetId), toolSetSize(toolSetSize), toolCoust(toolCoust) {}
