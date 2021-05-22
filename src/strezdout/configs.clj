(ns strezdout.configs
  (:require [strezdout.osutil :refer [testers-canonize]]))

(let [configs'
   {:fibonacci {:generation [:clojure "generator.clj"]
                :tests {:tester "tester"
                        :testee "testee"
                        :executable "fibonacci"}
                :copy ["fibonacci.cpp" "CMakeLists.txt"]}
    :wordnet {:generation [:binary "test_gen"]
              :tests {:tester "wordnet_tester"
                      :testee "wordnet_testee"
                      :executable "wordnet"}
              :copy ["wordnet.cpp" "CMakeLists.txt" "words.txt" "synsets.txt" "hypernyms.txt"]}
    :8-puzzle {:generation [:binary "generator"]
               :tests {:tester "8puzzle_tester"
                       :testee "8puzzle_testee"
                       :executable "8puzzle"}
               :copy ["8puzzle.cpp" "CMakeLists.txt"]}}]
  (def configs (into {} (for [[task
                               {[_ generator] :generation
                                {:keys [library include]} :tests
                                copy :copy :as task-config}] configs'
                              :let [canonize-task (partial testers-canonize (name task))]]
                          [task (-> task-config
                                    (assoc-in [:generation 1] (canonize-task generator))
                                    (assoc :copy (mapv canonize-task copy)))]))))
