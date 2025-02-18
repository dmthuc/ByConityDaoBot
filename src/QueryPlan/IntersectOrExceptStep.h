#pragma once
#include <QueryPlan/IQueryPlanStep.h>
#include <Parsers/ASTSelectIntersectExceptQuery.h>


namespace DB
{

class IntersectOrExceptStep : public IQueryPlanStep
{
using Operator = ASTSelectIntersectExceptQuery::Operator;

public:
    /// max_threads is used to limit the number of threads for result pipeline.
    IntersectOrExceptStep(DataStreams input_streams_, Operator operator_, size_t max_threads_ = 0);

    String getName() const override { return "IntersectOrExcept"; }

    Type getType() const override { return Type::IntersectOrExcept; }

    void setInputStreams(const DataStreams & input_streams_) override { input_streams = input_streams_; }

    std::shared_ptr<IQueryPlanStep> copy(ContextPtr) const override { return std::make_shared<IntersectOrExceptStep>(input_streams, current_operator, max_threads); }

    QueryPipelinePtr updatePipeline(QueryPipelines pipelines, const BuildQueryPipelineSettings & settings) override;

    void serialize(WriteBuffer & buf) const override;
    static QueryPlanStepPtr deserialize(ReadBuffer & buf, ContextPtr context);
    
    void describePipeline(FormatSettings & settings) const override;
    
    String getOperator() const;

private:
    Block header;
    Operator current_operator;
    size_t max_threads;
    Processors processors;
};

}
