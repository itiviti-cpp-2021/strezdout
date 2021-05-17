(ns strezdout.bobthebuildr
  (:require [strezdout.osutil :refer :all]
            [clojure.tools.logging :as logging]
            [clojure.java.shell :refer [sh]]))

(defn prepare-confs! "Prepare configs for testing by building the testers" [logger configs]
  (mkdir! (canonize "built")) ; directory for all built configs
  (doseq [[task' {{:keys [tester executable library include]} :tests copy :copy}] configs
          :let [task (name task')
                task-path (canonize "built" task)
                tester-include-path (canonize task-path "include")
                task-build-path (canonize task-path "build")]]
    (logging/info ::prepare-confs "Preparing task" {:task task :path task-path})
    (run! mkdir! [task-path task-build-path]) ; directory for current built task
    (run! #(copy! % task-path) copy) ; copy sources, cmakelists etc
    (copy! include tester-include-path) ; copy the tester includes
    (copy! library task-build-path) ; copy the prebuilt library for single-use build
    (logging/info ::prepare-confs "Building task tester"
                  (cmake! task-build-path task-path)
                  (make! task-build-path executable))
    (copy! (canonize task-build-path executable) (canonize task-path tester))
    (rm! task-build-path)
    (rm! tester-include-path)))

(defn build! "Create temp directory, copy sources to it and build" [logger task' config sources includes]
  (let [{{:keys [testee executable]} :tests} config
        task (name task')
        directory (canonize (tmpdir!) task)
        task-path (canonize "built" task)
        build-path (canonize directory "build")]
    (logging/info ::build "Preparing testee" {:task task :path directory :task-path task-path})
    (copy! task-path directory) ; copy prebuilt stuff first
    (copy! includes (canonize directory "include")) ; then copy the includes + sources
    (copy! sources (canonize directory "src"))
    (mkdir! build-path)
    (logging/info ::build "Building"
                  (cmake! build-path directory)
                  (make! build-path executable))
    (copy! (canonize build-path executable) (canonize directory testee))
    (rm! build-path) ; finally cleanup and return the directory
    directory))

(defn launch-options "Get launch options for launching a specified generator" [[type path]]
  (case type
    :clojure ["lein" "exec" path]))
