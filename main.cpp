#include <iostream>
#include <vector>

#include "Metric.h"
#include "IDatasetStream.h"

void NeedToSet(algos::Primitive& primitive) {
    std::unordered_set<std::string> needs_to_be_set = primitive.GetNeededOptions();
    std::cout << "Need to set: ";
    for (auto const& el : needs_to_be_set) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

/*void FromVarMap(algos::Primitive& primitive, var_map var_map) {
    std::unordered_set<std::string> needed;
    while (!(needed = primitive.GetNeededOptions()).empty()) {
        for (auto const& el : needed) {
            if (var_map.find(el) != var_map.end()) {
                primitive.SetOption(el, var_map.at(el));
            }
            else {
                primitive.SetOption(el);
            }
        }
    }
}*/

int main() {
    // Get a (task, primitive) pair from the user. Get the primitive.
    // Remove the "task" and "primitive" options from var_map.
    // Parse according to the primitive's program_options.
    // Configure the primitive using FromVarMap.
    // Execute the primitive.

    // This is a dummy method to show that the configuration idea is sensible.
    std::unique_ptr<algos::Primitive> primitive = std::make_unique<algos::MetricVerifier>();

    std::cout << "Before fitting:" << std::endl;
    NeedToSet(*primitive);

    // FromVarMap(*primitive, var_map)
    primitive->SetOption("is_null_equal_null");
    std::unique_ptr<model::IDatasetStream> parser = std::make_unique<CSVParser>();
    primitive->Fit(*parser);

    // FromVarMap(*primitive, var_map)
    std::cout << "After fitting:" << std::endl;
    NeedToSet(*primitive);
    primitive->SetOption("dist_to_null_infinity");
    primitive->SetOption("lhs_indices", std::vector<unsigned int>{0, 2});
    primitive->SetOption("rhs_indices", std::vector<unsigned int>{1});
    primitive->SetOption("parameter");

    std::cout << "After setting main options:" << std::endl;
    NeedToSet(*primitive);
    primitive->SetOption("metric", std::string("cosine"));

    std::cout << "After setting metric:" << std::endl;
    NeedToSet(*primitive);
    primitive->SetOption("metric_algorithm", std::string("brute"));
    primitive->SetOption("q");

    std::cout << "Before processing:" << std::endl;
    NeedToSet(*primitive);
    std::cout << "\"Finished\" in: " << primitive->Execute() << "ms" << std::endl;
    std::cout << "Done!" << std::endl;

    std::cout << "rhs_indices should appear:" << std::endl;
    primitive->UnsetOption("rhs_indices");
    NeedToSet(*primitive);
    return 0;
}
