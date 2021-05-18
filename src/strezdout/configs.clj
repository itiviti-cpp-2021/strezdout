(ns strezdout.configs
  (:require [strezdout.osutil :refer [testers-canonize]]))

(let [configs'
   {:fibonacci {:generation [:clojure "generator.clj"]
                :tests {:tester "tester"
                        :testee "testee"
                        :executable "fibonacci"
                        :library "libfibonacci_lib.a"
                        :include "include"}
                :copy ["fibonacci.cpp" "CMakeLists.txt"]}
    :wordnet {:generation [:binary "test_gen"]
              :tests {:tester "wordnet_tester"
                      :testee "wordnet_testee"
                      :executable "wordnet"
                      :library "libwordnet_lib.a"
                      :include "include"}
              :copy ["wordnet.cpp" "CMakeLists.txt" "words.txt" "synsets.txt" "hypernyms.txt"]}}]
  (def configs (into {} (for [[task
                               {[_ generator] :generation
                                {:keys [library include]} :tests
                                copy :copy :as task-config}] configs'
                              :let [canonize-task (partial testers-canonize (name task))]]
                          [task (-> task-config
                                    (assoc-in [:generation 1] (canonize-task generator))
                                    (assoc-in [:tests :library] (canonize-task library))
                                    (assoc-in [:tests :include] (canonize-task include))
                                    (assoc :copy (mapv canonize-task copy)))]))))
