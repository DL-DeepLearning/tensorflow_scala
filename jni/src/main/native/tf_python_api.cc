/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tf_python_api.h"

#include "tf_c_api_internal.h"

namespace tensorflow {

void UpdateInput(TF_Graph* graph, TF_Operation* op, int index, TF_Output output) {
  mutex_lock l(graph->mu);
  const tensorflow::Edge* edge;
  Status s = op->node.input_edge(index, &edge);
  if (s.ok())
    graph->graph.RemoveEdge(edge);
  graph->graph.AddEdge(&output.oper->node, output.index, &op->node, index);
}

void AddControlInput(TF_Graph* graph, TF_Operation* op, TF_Operation* input) {
  mutex_lock l(graph->mu);
  graph->graph.AddControlEdge(&input->node, &op->node);
}

void ClearControlInputs(TF_Graph* graph, TF_Operation* op) {
  mutex_lock l(graph->mu);
  for (const auto* edge : op->node.in_edges()) {
    if (edge->IsControlEdge()) {
      graph->graph.RemoveEdge(edge);
    }
  }
}

void SetRequestedDevice(TF_Graph* graph, TF_Operation* op, const char* device) {
  mutex_lock l(graph->mu);
  op->node.set_requested_device(device);
}

}  // namespace tensorflow
