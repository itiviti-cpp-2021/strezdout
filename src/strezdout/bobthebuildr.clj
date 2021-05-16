(ns strezdout.bobthebuilder
  (:require [strezdout.osutil :refer :all]
            [clojure.java.shell :refer [sh]]))

(defn prepare-confs! "Prepare configs for testing by building the testers" [configs]
  (mkdir! (canonize "built")) ; directory for all built configs
  (doseq [[task' {{:keys [tester executable library]} :tester copy :copy}] configs
          :let [task (name task')
                task-path (canonize "built" task)
                task-build-path (canonize task-path "build")]]
    (run! mkdir! [task-path task-build-path]) ; directory for current built task
    (run! #(copy! % task-path) copy) ; copy sources, includes etc
    (copy! library task-build-path) ; copy the prebuilt library for single-use build
    (cmake! task-build-path task-path)
    (make! task-build-path executable) ; cmake and make the tester executable
    (copy! (canonize task-build-path executable) (canonize task-path tester))
    (rm! task-build-path)))

(defn build! "Create temp directory, copy sources to it and build" [task' config sources]
  (let [{{:keys [testee executable]} :tester} config
        task (name task')
        directory (canonize (tmpdir!) task)
        task-path (canonize "built" task)
        build-path (canonize directory "build")]
    (copy! task-path directory) ; copy prebuilt stuff first
    (copy! sources (canonize directory "src")) ; then copy the sources
    (mkdir! build-path)
    (cmake! build-path directory)
    (make! build-path executable) ; build our executable and copy it back
    (copy! (canonize build-path executable) (canonize directory testee))
    (rm! build-path) ; finally cleanup and return the directory
    directory))
