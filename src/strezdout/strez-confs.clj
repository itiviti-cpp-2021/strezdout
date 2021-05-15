(ns strezdout.strez-confs
  (:require [strezdout.fileutil :refer [testers-canonize]]))

(let [configs'
   {:fibonacci {:generator [:clojure "generator.clj"]
                :tester {:tester "tester"
                         :testee "testee"
                         :executable "fibonacci"
                         :library "libfibonacci_lib.a"}
                :copy ["fibonacci.cpp" "include" "CMakeLists.txt"]}}]
  (def configs (into {} (for [[task
                               {[_ generator] :generator
                                {library :library} :tester
                                copy :copy :as task-config}] configs'
                              :let [canonize-task (partial testers-canonize (name task))]]
                          [task (-> task-config
                                    (assoc-in [:generator 1] (canonize-task generator))
                                    (assoc-in [:tester :library] (canonize-task library))
                                    (assoc :copy (mapv canonize-task copy)))]))))
