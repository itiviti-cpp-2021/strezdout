(ns strezdout.bobthebuilder
  (:require [strezdout.fileutil :refer :all]
            [clojure.java.shell :refer [sh]]))

(defn prepare-confs! "Prepare configs for testing by building the testers" [configs]
  (mkdir (canonize "built")) ; directory for all built configs
  (doseq [[task' {{:keys [tester executable library]} :tester copy :copy}] configs
          :let [task (name task')
                task-path (canonize "built" task)
                task-build-path (canonize task-path "build")]]
    (run! mkdir! [task-path task-build-path]) ; directory for current built task
    (run! #(copy! % task-path) copy) ; copy sources, includes etc
    (copy! library task-build-path) ; copy the prebuilt library for single-use build
    (cmake! task-build-path (testers-canonize task))
    (make! task-build-path executable) ; cmake and make the tester executable
    (copy! (canonize task-build-path executable) (canonize task-path tester))
    (rm! task-build-path)))
